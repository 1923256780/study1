/**
 ****************************************************************************************
 *
 * @file nvds.h
 *
 * @brief Non Volatile Data Storage (NVDS) driver
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 ****************************************************************************************
 */
#ifndef _NVDS_H_
#define _NVDS_H_

/**
 ****************************************************************************************
 * @addtogroup NVDS
 * @ingroup COMMON
 * @brief Non Volatile Data Storage (NVDS)
 *
 *   Parameters management
 *   there are two compilation options:
 *     + NVDS_8BIT_TAGLENGTH :
 *       if set, each TAG has a maximum length of 256 bytes
 *       if not set, each TAG has a maximum length of 65536 bytes
 *     + NVDS_PACKED :
 *       if not set, all the TAG header structures and TAG data contents are stored with an
 *       alignment on 32 bit boundary
 *       if set, all the TAG header structures and TAG data contents are stored
 *       consecutively without gaps (as would be a structure with pragma packed)
 *     + NVDS_READ_WRITE :
 *       if not set, only GET action on TAGs is provided.
 *       if set, PUT/DEL/LOCK actions are provided in addition of GET action.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>           // boolean definition
#include <stdint.h>            // integer definition


/*
 * DEFINES
 ****************************************************************************************
 */

/// NVDS is defined as read-write
#define NVDS_READ_WRITE          1

/// NVDS is defined as packed
#define NVDS_PACKED              1

/// NVDS has 8-bit length tags
#define NVDS_8BIT_TAGLENGTH      1

/// Type of the tag length (8 or 16 bits)
#if (NVDS_8BIT_TAGLENGTH)
typedef uint8_t  nvds_tag_len_t;
#else
typedef uint16_t nvds_tag_len_t;
#endif // NVDS_8BIT_TAGLENGTH

/// Environment structure of the NVDS module
struct nvds_env_tag
{
    /// Function to read the device Address being in the NVDS memory space
    void  (*read)(uint32_t const                    address,
                  uint32_t const                    length,
                  uint8_t* const                    buf);
    /// Function to write the device Address being in the NVDS memory space
    void (*write)(uint32_t const                    address,
                  uint32_t const                    length,
                  uint8_t* const                    buf);
    /// Function to erase the entire NVDS memory space
    void (*erase)(uint32_t const                    address,
                  uint32_t const                    length);

    /// NVDS base pointer
    uint8_t *nvds_space;

    /// Total size of the NVDS area
    uint32_t   total_size;

    /// Flash ID
    uint8_t flash_id;
};


/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

/// Possible Returned Status
enum NVDS_STATUS
{
    /// NVDS status OK
    NVDS_OK,
    /// generic NVDS status KO
    NVDS_FAIL,
    /// NVDS TAG unrecognized
    NVDS_TAG_NOT_DEFINED,
    /// No space for NVDS
    NVDS_NO_SPACE_AVAILABLE,
    /// Length violation
    NVDS_LENGTH_OUT_OF_RANGE,
    /// NVDS parameter locked
    NVDS_PARAM_LOCKED,
    /// NVDS corrupted
    NVDS_CORRUPT
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize NVDS.
 * @return NVDS_OK
 ****************************************************************************************
 */
uint8_t nvds_init(void);

/**
 ****************************************************************************************
 * @brief 查找特定的标签，并且如果找到并且匹配（在长度上），则返回标签的DATA部分。
 *
 * 如果长度不匹配，则仍然填充TAG头结构，以便调用者能够检查TAG的实际长度。
 *
 * @param[in]  tag     要查找的标签，其DATA将被检索
 * @param[in]  length  预期的标签长度
 * @param[out] buf     由调用者分配的缓冲区的指针，用于填充标签的DATA部分
 *
 * @return  NVDS_OK                  执行了读操作
 *          NVDS_LENGTH_OUT_OF_RANGE 传入参数的长度与标签的长度不同
 ****************************************************************************************
 */
uint8_t nvds_get(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf);

#if (NVDS_READ_WRITE == 1)

/**
 ****************************************************************************************
 * @brief 查找特定的标签并删除它（状态设置为无效）
 *
 * 实现注意事项
 * 1. 写函数调用的返回状态未处理
 *
 * @param[in]  tag    要标记为删除的标签
 *
 * @return NVDS_OK                找到并删除的标签
 *         NVDS_PARAM_LOCKED    找到标签但无法删除，因为它被锁定
 *         (others)        函数调用 @ref nvds_browse_tag 的返回值
 ****************************************************************************************
 */
uint8_t nvds_del(uint8_t tag);


/**
 ****************************************************************************************
 * @brief 查找特定的TAG并锁定它（状态锁位设置为LOCK）。
 *
 * 写函数调用返回的状态未处理
 *
 * @param[in]  tag    要标记为锁定的TAG
 *
 * @return NVDS_OK    找到并锁定的TAG
 *         (others)        函数调用 @ref nvds_browse_tag 的返回值
 ****************************************************************************************
 */
uint8_t nvds_lock(uint8_t tag);

/**
 ****************************************************************************************
 * @brief 此函数将特定的TAG添加到NVDS中。
 *
 * 步骤：
 * 1) 解析所有的TAG：
 *    1.1) 计算所有有效TAG的总大小
 *    1.2) 擦除具有相同ID的现有TAG
 *    1.3) 检查是否可以在EEPROM中使用相同的TAG区域
 *    1.4) 检查TAG是否被锁定
 * 2) 如果必须将新TAG添加到NVDS的末尾（无法使用相同的区域）：
 *    2.1) 分配适当数量的内存
 *    2.2) 清除NVDS
 *    2.3) 释放分配的内存
 *    2.4) 检查现在是否有足够的空间来容纳新的TAG，否则返回NO_SPACE_AVAILABLE
 * 3) 添加新的TAG
 *
 * @param[in]  tag     要查找的TAG，其数据将被检索
 * @param[in]  length  预期的TAG长度
 * @param[in]  buf     指向包含要添加到NVDS的TAG的数据部分的缓冲区的指针
 *
 * @return NVDS_OK                  新TAG正确写入NVDS
 *         NVDS_PARAM_LOCKED        新TAG试图覆盖已锁定的TAG
 *         NO_SPACE_AVAILABLE       新TAG无法适应NVDS中的可用空间
 ****************************************************************************************
 */
uint8_t nvds_put(uint8_t tag, nvds_tag_len_t length, uint8_t *buf);


#endif //(NVDS_READ_WRITE == 1)

/// @} NVDS

#endif // _NVDS_H_

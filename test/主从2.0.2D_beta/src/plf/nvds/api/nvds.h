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
 * @brief �����ض��ı�ǩ����������ҵ�����ƥ�䣨�ڳ����ϣ����򷵻ر�ǩ��DATA���֡�
 *
 * ������Ȳ�ƥ�䣬����Ȼ���TAGͷ�ṹ���Ա�������ܹ����TAG��ʵ�ʳ��ȡ�
 *
 * @param[in]  tag     Ҫ���ҵı�ǩ����DATA��������
 * @param[in]  length  Ԥ�ڵı�ǩ����
 * @param[out] buf     �ɵ����߷���Ļ�������ָ�룬��������ǩ��DATA����
 *
 * @return  NVDS_OK                  ִ���˶�����
 *          NVDS_LENGTH_OUT_OF_RANGE ��������ĳ������ǩ�ĳ��Ȳ�ͬ
 ****************************************************************************************
 */
uint8_t nvds_get(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf);

#if (NVDS_READ_WRITE == 1)

/**
 ****************************************************************************************
 * @brief �����ض��ı�ǩ��ɾ������״̬����Ϊ��Ч��
 *
 * ʵ��ע������
 * 1. д�������õķ���״̬δ����
 *
 * @param[in]  tag    Ҫ���Ϊɾ���ı�ǩ
 *
 * @return NVDS_OK                �ҵ���ɾ���ı�ǩ
 *         NVDS_PARAM_LOCKED    �ҵ���ǩ���޷�ɾ������Ϊ��������
 *         (others)        �������� @ref nvds_browse_tag �ķ���ֵ
 ****************************************************************************************
 */
uint8_t nvds_del(uint8_t tag);


/**
 ****************************************************************************************
 * @brief �����ض���TAG����������״̬��λ����ΪLOCK����
 *
 * д�������÷��ص�״̬δ����
 *
 * @param[in]  tag    Ҫ���Ϊ������TAG
 *
 * @return NVDS_OK    �ҵ���������TAG
 *         (others)        �������� @ref nvds_browse_tag �ķ���ֵ
 ****************************************************************************************
 */
uint8_t nvds_lock(uint8_t tag);

/**
 ****************************************************************************************
 * @brief �˺������ض���TAG��ӵ�NVDS�С�
 *
 * ���裺
 * 1) �������е�TAG��
 *    1.1) ����������ЧTAG���ܴ�С
 *    1.2) ����������ͬID������TAG
 *    1.3) ����Ƿ������EEPROM��ʹ����ͬ��TAG����
 *    1.4) ���TAG�Ƿ�����
 * 2) ������뽫��TAG��ӵ�NVDS��ĩβ���޷�ʹ����ͬ�����򣩣�
 *    2.1) �����ʵ��������ڴ�
 *    2.2) ���NVDS
 *    2.3) �ͷŷ�����ڴ�
 *    2.4) ��������Ƿ����㹻�Ŀռ��������µ�TAG�����򷵻�NO_SPACE_AVAILABLE
 * 3) ����µ�TAG
 *
 * @param[in]  tag     Ҫ���ҵ�TAG�������ݽ�������
 * @param[in]  length  Ԥ�ڵ�TAG����
 * @param[in]  buf     ָ�����Ҫ��ӵ�NVDS��TAG�����ݲ��ֵĻ�������ָ��
 *
 * @return NVDS_OK                  ��TAG��ȷд��NVDS
 *         NVDS_PARAM_LOCKED        ��TAG��ͼ������������TAG
 *         NO_SPACE_AVAILABLE       ��TAG�޷���ӦNVDS�еĿ��ÿռ�
 ****************************************************************************************
 */
uint8_t nvds_put(uint8_t tag, nvds_tag_len_t length, uint8_t *buf);


#endif //(NVDS_READ_WRITE == 1)

/// @} NVDS

#endif // _NVDS_H_

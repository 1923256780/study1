/*
 * @Author: LHB
 * @Date: 2023-06-01 14:21:25
 * @LastEditors: LHB
 * @LastEditTime: 2023-07-26 11:05:22
 * @Description: 
 * 1
 */
#ifndef List_hb_H__
#define List_hb_H__

#define MAX_TIMERS 100

// 链表结构体定义
struct node {
    int data;
    struct node *next;
};

void test_list_main(void);
void insert_node_at_head(struct node **head_ref, int data);
void insert_node_at_tail(struct node **head_ref, int data);
void delete_node(struct node **head_ref, int data);
void print_list(struct node *head);



typedef void (*timer_callback)(void);

typedef enum {
    TIMER_IDLE,
    TIMER_RUNNING,
    TIMER_PAUSED,
    TIMER_STOPPED,
    TIMER_ERROR,
    TIMER_DISABLED
} timer_state;


typedef struct {
    int task_id;
    int repeat_count;
    int repeat_count_target;
    int interval;
    int current_count;
    timer_callback callback;
    timer_state state;
} timer_task;
void TimeList_main(void);
void timer_tick(timer_task* task);
int start_timer(timer_task* task);
void pause_timer(timer_task* task);
void restart_timer(timer_task* task);
void stop_timer(timer_task* task);
void reset_timer(timer_task* task);
void delete_timer_task(timer_task* task);
timer_task* create_timer_task(int repeat_count, int interval, timer_callback callback);


#endif


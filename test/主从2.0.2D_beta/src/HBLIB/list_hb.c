/*
 * @Author: LHB
 * @Date: 2023-06-01 14:21:11
 * @LastEditors: LHB
 * @LastEditTime: 2023-07-26 14:14:27
 * @Description: 
 * 1
 */
#include "list_hb.h"


#include "ke_mem.h"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>

// 创建一个新节点
struct node* create_node(int data) {
    struct node *new_node = (struct node*)ke_malloc(sizeof(struct node),KE_MEM_ATT_DB);;
    if (new_node == NULL) {
        uart_printf("Error: Unable to allocate memory \r\n");
        // exit(1);
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// 在链表头部插入一个节点
void insert_node_at_head(struct node **head_ref, int data) {
    struct node *new_node = create_node(data);
    new_node->next = *head_ref;
    *head_ref = new_node;
}

// 在链表尾部插入一个节点
void insert_node_at_tail(struct node **head_ref, int data) {
    struct node *new_node = create_node(data);
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }
    struct node *cur_node = *head_ref;
    while (cur_node->next != NULL) {
        cur_node = cur_node->next;
    }
    cur_node->next = new_node;
}

// 从链表中删除一个节点
void delete_node(struct node **head_ref, int data) {
    if (*head_ref == NULL) {
        return;
    }
    if ((*head_ref)->data == data) {
        struct node *temp = *head_ref;
        *head_ref = (*head_ref)->next;
        ke_free(temp);
        return;
    }
    struct node *cur_node = *head_ref;
    //HB_临时注释 判断为不是最后一个，且 ，数据不是想找的那个，就移到下一个
    while (cur_node->next != NULL && cur_node->next->data != data) {
        cur_node = cur_node->next;
    }
    //HB_临时注释 已经到最后一个，并未找到目标数据，直接结束
    if (cur_node->next == NULL) {
        return;
    }
    struct node *temp = cur_node->next;
    cur_node->next = cur_node->next->next;
    ke_free(temp);
}

// 打印链表
void print_list(struct node *head) {
    while (head != NULL) {
        uart_printf("%d ", head->data);
        head = head->next;
    }
    uart_printf("\r\n");
}

void test_list_main(void) {
    struct node *head = NULL;

    // 插入节点
    insert_node_at_head(&head, 1);
    insert_node_at_head(&head, 2);
    insert_node_at_tail(&head, 3);

    // 打印链表
    print_list(head); // output: 2 1 3

    // 删除节点
    delete_node(&head, 2);

    // 打印链表
    print_list(head); // output: 1 3

}


//------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
//=========================================fitshow 协议=======================================================================================================//HB-注释
void FitshowProtocol_CB()
{
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释





//================================================================================================================================================//HB-注释
timer_task* timer_test;


timer_task* create_timer_task(int repeat_count, int interval, timer_callback callback) {
    timer_task* task = (timer_task*) ke_malloc(sizeof(timer_task),KE_MEM_NON_RETENTION);
    if (task == NULL) {
        return NULL;
    }

    task->task_id = rand();  // 生成随机的任务ID
    task->repeat_count = repeat_count;
    task->interval = interval;
    task->repeat_count_target = repeat_count;
    task->current_count = 0;
    task->callback = callback;
    task->state = TIMER_IDLE;

    return task;
}

int start_timer(timer_task* task) {
    if (task == NULL) {
        uart_printf("time is error\r\n");
        return 1;
    }

    task->state = TIMER_RUNNING;
    return 0;
}

void pause_timer(timer_task* task) {
    if (task == NULL) {
        uart_printf("time is error\r\n");
        return;
    }

    task->state = TIMER_PAUSED;
}

void restart_timer(timer_task* task) {
    if (task == NULL) {
        uart_printf("time is error\r\n");
        return;
    }

    task->state = TIMER_RUNNING;
    task->current_count = 0;
}

void stop_timer(timer_task* task) {
    if (task == NULL) {
        uart_printf("time is error\r\n");
        return;
    }

    task->state = TIMER_STOPPED;
}

void reset_timer(timer_task* task) {
    if (task == NULL) {
        uart_printf("time is error\r\n");
        return;
    }

    task->repeat_count = task->repeat_count_target;
    task->current_count = 0;
    task->state = TIMER_IDLE;
}

void delete_timer_task(timer_task* task) {

    if (task != NULL) {
        ke_free(task);
    }

}

void timer_tick(timer_task* task) {
    if (task == NULL || task->state != TIMER_RUNNING || task->task_id == 0) {
        return;
    }

    task->current_count++;

    if (task->current_count == task->interval)
    {
        task->current_count = 0;
        if(task->repeat_count>0){
            task->repeat_count--;
        }
        if(task->callback != NULL){
            task->callback();
        }
        if(task->repeat_count==0){
            task->state = TIMER_IDLE;
            task->current_count = 0;
        }
    }
}

void timer_callback_example() {

}

void TimeList_main() {


}

//================================================================================================================================================//HB-注释



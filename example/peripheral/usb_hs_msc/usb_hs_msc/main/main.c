#include <stdio.h>
#include "log.h"
#include "core_802.h"
#include "mcu_phy_bumbee.h"
#include "common.h"
#include "misc.h"
#include "uart.h"
#include "usb_hal_hcd.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "file_operation.h"

extern FATFS USBH_fatfs;

USBH_HandleTypeDef hUSBHost;
MSC_DEMO_StateMachine msc_demo;
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
char USBDISKPath[4]; /* USB Host logical drive path */

static int32_t recurseLevel = -1;

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
    switch (id)
    {
    case HOST_USER_SELECT_CONFIGURATION:
        break;

    case HOST_USER_DISCONNECTION:
        if ((Appli_state == APPLICATION_DISCONNECT) || (Appli_state == APPLICATION_IDLE))
        {
            return;
        }
        Appli_state = APPLICATION_DISCONNECT;
        if (f_mount(NULL, "", 0) != FR_OK)
        {
            LOG_ERROR("ERROR : Cannot DeInitialize FatFs! \n");
        }
        if (FATFS_UnLinkDriver(USBDISKPath) != 0)
        {
            LOG_ERROR("ERROR : Cannot UnLink FatFS Driver! \n");
        }
        printk("umount\n");
        break;

    case HOST_USER_CLASS_ACTIVE:
        Appli_state = APPLICATION_READY;
        break;

    case HOST_USER_CONNECTION:
        if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
        {
            if (f_mount(&USBH_fatfs, "", 0) != FR_OK)
            {
                LOG_ERROR("ERROR : Cannot Initialize FatFs! \n");
            }
        }
        printk("mount\n");
        break;

    default:
        break;
    }
}

/**
  * @brief  Displays disk content.
  * @param  path: Pointer to root path
  * @param  recu_level: Disk content level
  * @retval Operation result
  */
FRESULT Explore_Disk(char *path, uint8_t recu_level)
{
    FRESULT res = FR_OK;
    FILINFO fno;
    DIR dir;
    char tmp[14];
    //uint8_t line_idx = 0;

    recurseLevel++;
    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        while (USBH_MSC_IsReady(&hUSBHost))
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }
            if (fno.fname[0] == '.')
            {
                continue;
            }

            strcpy(tmp, fno.fname);

            if (recu_level == 1)
            {
                printk("   |__");
            }
            else if (recu_level == 2)
            {
                printk("   |   |__");
            }
            if (fno.fattrib & AM_DIR)
            {
                strcat(tmp, "\n");
                printk((void *)tmp);
                Explore_Disk(fno.fname, 2);
            }
            else
            {
                strcat(tmp, "\n");
                printk((void *)tmp);
            }

            if ((fno.fattrib & AM_DIR) && (recu_level == 2))
            {
                Explore_Disk(fno.fname, 2);
            }
        }
        f_closedir(&dir);
    }

    if (--recurseLevel == -1)
    {
        printk("> Select an operation to Continue.\n");
    }

    return res;
}

volatile int32_t display_flag = 0;

void MSC_MenuProcess(void)
{
    if (display_flag == 0)
    {

        switch (msc_demo.state)
        {
        case MSC_DEMO_START:
            if (Appli_state == APPLICATION_READY)
            {
                //printk("Explore Disk #\n");
                //Explore_Disk("0:/", 1);
                msc_demo.state = MSC_DEMO_FILE_OPERATIONS;
                /* Prevent debounce effect for user key */
                USBH_Delay(800);
            }
            break;
        case MSC_DEMO_FILE_OPERATIONS:
            /* Read and Write File Here */
            if (Appli_state == APPLICATION_READY)
            {
                MSC_File_Operations();
                msc_demo.state = MSC_DEMO_EXPLORER;
                USBH_Delay(800);
            }
            break;

        case MSC_DEMO_EXPLORER:
            /* Display disk content */
            if (Appli_state == APPLICATION_READY)
            {
                printk("Explore Disk #\n");
                Explore_Disk("0:/", 1);
                msc_demo.state = MSC_DEMO_START;
                USBH_Delay(400);
                display_flag = 1;
            }
            break;
        default:
            break;
        }
    }
    if (Appli_state == APPLICATION_DISCONNECT)
    {
        Appli_state = APPLICATION_IDLE;
        printk("MSC device disconnected!\n");
        msc_demo.state = MSC_DEMO_START;
        msc_demo.select = 0;
        display_flag = 0;
    }
}

int main(void)
{
    uart_init();
    TEST_START("USB MSC");
 
    NVIC_SetPriority(USB_IRQn, IRQ_PRIO_HAL);
    NVIC_EnableIRQ(USB_IRQn);
    USBH_Init(&hUSBHost, USBH_UserProcess, 0);

    /* Add Supported Class */
    USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);

    /* Start Host Process */
    USBH_Start(&hUSBHost);

    while (1)
    {
        USBH_Process(&hUSBHost);

        MSC_MenuProcess()

        //USBH_Delay(1000);
		
        //printk("time %d\n", hUSBHost.Timer);
    }
}
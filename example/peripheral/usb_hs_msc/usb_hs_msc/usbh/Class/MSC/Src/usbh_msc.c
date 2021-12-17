#include "usbh_msc.h"
#include "usbh_msc_bot.h"
#include "usbh_msc_scsi.h"

static USBH_StatusTypeDef USBH_MSC_InterfaceInit(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_MSC_InterfaceDeInit(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_MSC_Process(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_MSC_ClassRequest(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_MSC_SOFProcess(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_MSC_RdWrProcess(USBH_HandleTypeDef *phost, uint8_t lun);

USBH_ClassTypeDef  USBH_msc =
{
    "MSC",
    USB_MSC_CLASS,
    USBH_MSC_InterfaceInit,
    USBH_MSC_InterfaceDeInit,
    USBH_MSC_ClassRequest,
    USBH_MSC_Process,
    USBH_MSC_SOFProcess,
    NULL,
};

/**
  * @brief  USBH_MSC_InterfaceInit
  *         The function init the MSC class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_MSC_InterfaceInit(USBH_HandleTypeDef *phost)
{
    USBH_StatusTypeDef status;
    uint8_t interface;
    MSC_HandleTypeDef *MSC_Handle;

    interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, MSC_TRANSPARENT, MSC_BOT);

    if ((interface == 0xFFU) || (interface >= USBH_MAX_NUM_INTERFACES)) /* Not Valid Interface */
    {
        USBH_DbgLog("Cannot Find the interface for %s class.", phost->pActiveClass->Name);
        return USBH_FAIL;
    }

    status = USBH_SelectInterface(phost, interface);

    if (status != USBH_OK)
    {
        return USBH_FAIL;
    }

    phost->pActiveClass->pData = (MSC_HandleTypeDef *)USBH_malloc(sizeof(MSC_HandleTypeDef));
    MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    if (MSC_Handle == NULL)
    {
        USBH_DbgLog("Cannot allocate memory for MSC Handle");
        return USBH_FAIL;
    }

    /* Initialize msc handler */
    USBH_memset(MSC_Handle, 0, sizeof(MSC_HandleTypeDef));

    if (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80U)
    {
        MSC_Handle->InEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress) & 0x7f;
        MSC_Handle->InEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
    }
    else
    {
        MSC_Handle->OutEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress);
        MSC_Handle->OutEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
    }


    USBH_LL_Set_EndPoint_MAX_PKT_SZ(phost,
                                    phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x7FU,
                                    phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize);

    if (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress & 0x80U)
    {
        MSC_Handle->InEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress) & 0x7f;
        MSC_Handle->InEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].wMaxPacketSize;
    }
    else
    {
        MSC_Handle->OutEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress);
        MSC_Handle->OutEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].wMaxPacketSize;
    }

    USBH_LL_Set_EndPoint_MAX_PKT_SZ(phost,
                                    phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress & 0x7FU,
                                    phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].wMaxPacketSize);

    MSC_Handle->state = MSC_INIT;
    MSC_Handle->error = MSC_OK;
    MSC_Handle->req_state = MSC_REQ_IDLE;

    USBH_UsrLog("InEp %d, OutEp %d\n", MSC_Handle->InEp, MSC_Handle->OutEp);

    USBH_MSC_BOT_Init(phost);

    USBH_LL_SetToggle(phost, MSC_Handle->InEp, 0U);
    USBH_LL_SetToggle(phost, MSC_Handle->OutEp, 0U);

    return USBH_OK;
}

/**
  * @brief  USBH_MSC_InterfaceDeInit
  *         The function DeInit the Pipes used for the MSC class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_MSC_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    if (MSC_Handle->OutEp)
    {
        MSC_Handle->OutEp = 0U;     /* Reset the Channel as Free */
    }

    if (MSC_Handle->InEp)
    {
        MSC_Handle->InEp = 0U;     /* Reset the Channel as Free */
    }

    if (phost->pActiveClass->pData)
    {
        USBH_free(phost->pActiveClass->pData);
        phost->pActiveClass->pData = 0U;
    }

    return USBH_OK;
}

/**
  * @brief  USBH_MSC_ClassRequest
  *         The function is responsible for handling Standard requests
  *         for MSC class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_MSC_ClassRequest(USBH_HandleTypeDef *phost)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    USBH_StatusTypeDef status = USBH_BUSY;
    uint8_t i;

    /* Switch MSC REQ state machine */
    switch (MSC_Handle->req_state)
    {
    case MSC_REQ_IDLE:
    case MSC_REQ_GET_MAX_LUN:
        /* Issue GetMaxLUN request */
        status = USBH_MSC_BOT_REQ_GetMaxLUN(phost, &MSC_Handle->max_lun);

        /* When devices do not support the GetMaxLun request, this should
           be considred as only one logical unit is supported */
        if (status == USBH_NOT_SUPPORTED)
        {
            MSC_Handle->max_lun = 0U;
            status = USBH_OK;
        }

        if (status == USBH_OK)
        {
            MSC_Handle->max_lun = (MSC_Handle->max_lun > MAX_SUPPORTED_LUN) ? MAX_SUPPORTED_LUN : (MSC_Handle->max_lun + 1U);
            USBH_UsrLog("Number of supported LUN: %d", MSC_Handle->max_lun);

            for (i = 0U; i < MSC_Handle->max_lun; i++)
            {
                MSC_Handle->unit[i].prev_ready_state = USBH_FAIL;
                MSC_Handle->unit[i].state_changed = 0U;
            }
        }
        break;

    case MSC_REQ_ERROR:
        /* a Clear Feature should be issued here */
        if (USBH_ClrFeature(phost, 0x00U) == USBH_OK)
        {
            MSC_Handle->req_state = MSC_Handle->prev_req_state;
        }
        break;

    default:
        break;
    }

    return status;
}

/**
  * @brief  USBH_MSC_Process
  *         The function is for managing state machine for MSC data transfers
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_MSC_Process(USBH_HandleTypeDef *phost)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    USBH_StatusTypeDef error = USBH_BUSY;
    USBH_StatusTypeDef scsi_status = USBH_BUSY;
    USBH_StatusTypeDef ready_status = USBH_BUSY;

    switch (MSC_Handle->state)
    {
    case MSC_INIT:

        if (MSC_Handle->current_lun < MSC_Handle->max_lun)
        {

            MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_NOT_READY;
            /* Switch MSC REQ state machine */
            switch (MSC_Handle->unit[MSC_Handle->current_lun].state)
            {
            case MSC_INIT:
                USBH_UsrLog("LUN #%d: ", MSC_Handle->current_lun);
                MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_READ_INQUIRY;
                MSC_Handle->timer = phost->Timer;
                break;
            case MSC_READ_INQUIRY:
                scsi_status = USBH_MSC_SCSI_Inquiry(phost, (uint8_t)MSC_Handle->current_lun, &MSC_Handle->unit[MSC_Handle->current_lun].inquiry);

                if (scsi_status == USBH_OK)
                {
                    USBH_UsrLog("Inquiry Vendor  : %s", MSC_Handle->unit[MSC_Handle->current_lun].inquiry.vendor_id);
                    USBH_UsrLog("Inquiry Product : %s", MSC_Handle->unit[MSC_Handle->current_lun].inquiry.product_id);
                    USBH_UsrLog("Inquiry Version : %s", MSC_Handle->unit[MSC_Handle->current_lun].inquiry.revision_id);
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_TEST_UNIT_READY;
                }
                if (scsi_status == USBH_FAIL)
                {
                    USBH_UsrLog("MSC_READ_INQUIRY Fail\n");
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_REQUEST_SENSE;
                }
                else
                {
                    if (scsi_status == USBH_UNRECOVERED_ERROR)
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_IDLE;
                        MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_ERROR;
                        USBH_UsrLog("MSC_READ_INQUIRY Error\n");
                    }
                }
                break;
            case MSC_TEST_UNIT_READY:
                ready_status = USBH_MSC_SCSI_TestUnitReady(phost, (uint8_t)MSC_Handle->current_lun);

                if (ready_status == USBH_OK)
                {
                    if (MSC_Handle->unit[MSC_Handle->current_lun].prev_ready_state != USBH_OK)
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state_changed = 1U;
                        USBH_UsrLog("MSC Device ready");
                    }
                    else
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state_changed = 0U;
                    }
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_READ_CAPACITY10;
                    MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_OK;
                    MSC_Handle->unit[MSC_Handle->current_lun].prev_ready_state = USBH_OK;
                }
                if (ready_status == USBH_FAIL)
                {
                    /* Media not ready, so try to check again during 10s */
                    if (MSC_Handle->unit[MSC_Handle->current_lun].prev_ready_state != USBH_FAIL)
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state_changed = 1U;
                        USBH_UsrLog("MSC Device NOT ready");
                    }
                    else
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state_changed = 0U;
                    }
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_REQUEST_SENSE;
                    MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_NOT_READY;
                    MSC_Handle->unit[MSC_Handle->current_lun].prev_ready_state = USBH_FAIL;
                }
                else
                {
                    if (ready_status == USBH_UNRECOVERED_ERROR)
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_IDLE;
                        MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_ERROR;
                    }
                }
                break;

            case MSC_READ_CAPACITY10:
                scsi_status = USBH_MSC_SCSI_ReadCapacity(phost, (uint8_t)MSC_Handle->current_lun, &MSC_Handle->unit[MSC_Handle->current_lun].capacity) ;

                if (scsi_status == USBH_OK)
                {
                    if (MSC_Handle->unit[MSC_Handle->current_lun].state_changed == 1U)
                    {
                        USBH_UsrLog("MSC Device capacity : %lu Bytes", \
                                    (int32_t)(MSC_Handle->unit[MSC_Handle->current_lun].capacity.block_nbr * MSC_Handle->unit[MSC_Handle->current_lun].capacity.block_size));
                        USBH_UsrLog("Block number : %lu", (int32_t)(MSC_Handle->unit[MSC_Handle->current_lun].capacity.block_nbr));
                        USBH_UsrLog("Block Size   : %lu", (int32_t)(MSC_Handle->unit[MSC_Handle->current_lun].capacity.block_size));
                    }
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_IDLE;
                    MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_OK;
                    MSC_Handle->current_lun++;
                }
                else if (scsi_status == USBH_FAIL)
                {
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_REQUEST_SENSE;
                }
                else
                {
                    if (scsi_status == USBH_UNRECOVERED_ERROR)
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_IDLE;
                        MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_ERROR;
                    }
                }
                break;

            case MSC_REQUEST_SENSE:
                scsi_status = USBH_MSC_SCSI_RequestSense(phost, (uint8_t)MSC_Handle->current_lun, &MSC_Handle->unit[MSC_Handle->current_lun].sense);

                if (scsi_status == USBH_OK)
                {
                    if ((MSC_Handle->unit[MSC_Handle->current_lun].sense.key == SCSI_SENSE_KEY_UNIT_ATTENTION) ||
                            (MSC_Handle->unit[MSC_Handle->current_lun].sense.key == SCSI_SENSE_KEY_NOT_READY))
                    {

                        if ((phost->Timer - MSC_Handle->timer) < 10000U)
                        {
                            MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_TEST_UNIT_READY;
                            break;
                        }
                    }

                    USBH_UsrLog("Sense Key  : %x", MSC_Handle->unit[MSC_Handle->current_lun].sense.key);
                    USBH_UsrLog("Additional Sense Code : %x", MSC_Handle->unit[MSC_Handle->current_lun].sense.asc);
                    USBH_UsrLog("Additional Sense Code Qualifier: %x", MSC_Handle->unit[MSC_Handle->current_lun].sense.ascq);
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_IDLE;
                    MSC_Handle->current_lun++;
                }
                if (scsi_status == USBH_FAIL)
                {
                    USBH_UsrLog("MSC Device NOT ready");
                    MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_UNRECOVERED_ERROR;
                }
                else
                {
                    if (scsi_status == USBH_UNRECOVERED_ERROR)
                    {
                        MSC_Handle->unit[MSC_Handle->current_lun].state = MSC_IDLE;
                        MSC_Handle->unit[MSC_Handle->current_lun].error = MSC_ERROR;
                    }
                }
                break;

            case MSC_UNRECOVERED_ERROR:
                MSC_Handle->current_lun++;
                break;

            default:
                break;
            }
        }
        else
        {
            MSC_Handle->current_lun = 0U;
            MSC_Handle->state = MSC_IDLE;
            USBH_UsrLog("HOST_USER_CLASS_ACTIVE\n");
            phost->pUser(phost, HOST_USER_CLASS_ACTIVE);
        }
        break;

    case MSC_IDLE:
        error = USBH_OK;
        break;

    default:
        break;
    }
    return error;
}


/**
  * @brief  USBH_MSC_SOFProcess
  *         The function is for SOF state
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_MSC_SOFProcess(USBH_HandleTypeDef *phost)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(phost);

    return USBH_OK;
}
/**
  * @brief  USBH_MSC_RdWrProcess
  *         The function is for managing state machine for MSC I/O Process
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_MSC_RdWrProcess(USBH_HandleTypeDef *phost, uint8_t lun)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    USBH_StatusTypeDef error = USBH_BUSY ;
    USBH_StatusTypeDef scsi_status = USBH_BUSY ;

    /* Switch MSC REQ state machine */
    switch (MSC_Handle->unit[lun].state)
    {

    case MSC_READ:
        scsi_status = USBH_MSC_SCSI_Read(phost, lun, 0U, NULL, 0U);

        if (scsi_status == USBH_OK)
        {
            MSC_Handle->unit[lun].state = MSC_IDLE;
            error = USBH_OK;
        }
        else if (scsi_status == USBH_FAIL)
        {
            MSC_Handle->unit[lun].state = MSC_REQUEST_SENSE;
        }
        else
        {
            if (scsi_status == USBH_UNRECOVERED_ERROR)
            {
                MSC_Handle->unit[lun].state = MSC_UNRECOVERED_ERROR;
                error = USBH_FAIL;
            }
        }
        break;

    case MSC_WRITE:
        scsi_status = USBH_MSC_SCSI_Write(phost, lun, 0U, NULL, 0U);

        if (scsi_status == USBH_OK)
        {
            MSC_Handle->unit[lun].state = MSC_IDLE;
            error = USBH_OK;
        }
        else if (scsi_status == USBH_FAIL)
        {
            MSC_Handle->unit[lun].state = MSC_REQUEST_SENSE;
        }
        else
        {
            if (scsi_status == USBH_UNRECOVERED_ERROR)
            {
                MSC_Handle->unit[lun].state = MSC_UNRECOVERED_ERROR;
                error = USBH_FAIL;
            }
        }
        break;

    case MSC_REQUEST_SENSE:
        scsi_status = USBH_MSC_SCSI_RequestSense(phost, lun, &MSC_Handle->unit[lun].sense);

        if (scsi_status == USBH_OK)
        {
            USBH_UsrLog("Sense Key  : %x", MSC_Handle->unit[lun].sense.key);
            USBH_UsrLog("Additional Sense Code : %x", MSC_Handle->unit[lun].sense.asc);
            USBH_UsrLog("Additional Sense Code Qualifier: %x", MSC_Handle->unit[lun].sense.ascq);
            MSC_Handle->unit[lun].state = MSC_IDLE;
            MSC_Handle->unit[lun].error = MSC_ERROR;

            error = USBH_FAIL;
        }
        if (scsi_status == USBH_FAIL)
        {
            USBH_UsrLog("MSC Device NOT ready");
        }
        else
        {
            if (scsi_status == USBH_UNRECOVERED_ERROR)
            {
                MSC_Handle->unit[lun].state = MSC_UNRECOVERED_ERROR;
                error = USBH_FAIL;
            }
        }
        break;

    default:
        break;

    }
    return error;
}

/**
  * @brief  USBH_MSC_IsReady
  *         The function check if the MSC function is ready
  * @param  phost: Host handle
  * @retval USBH Status
  */
uint8_t  USBH_MSC_IsReady(USBH_HandleTypeDef *phost)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    uint8_t res;

    if ((phost->gState == HOST_CLASS) && (MSC_Handle->state == MSC_IDLE))
    {
        res = 1U;
    }
    else
    {
        res = 0U;
    }

    return res;
}

/**
  * @brief  USBH_MSC_GetMaxLUN
  *         The function return the Max LUN supported
  * @param  phost: Host handle
  * @retval logical Unit Number supported
  */
uint8_t  USBH_MSC_GetMaxLUN(USBH_HandleTypeDef *phost)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    if ((phost->gState == HOST_CLASS) && (MSC_Handle->state == MSC_IDLE))
    {
        return (uint8_t)MSC_Handle->max_lun;
    }

    return 0xFFU;
}

/**
  * @brief  USBH_MSC_UnitIsReady
  *         The function check whether a LUN is ready
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @retval Lun status (0: not ready / 1: ready)
  */
uint8_t  USBH_MSC_UnitIsReady(USBH_HandleTypeDef *phost, uint8_t lun)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    uint8_t res;

    if ((phost->gState == HOST_CLASS) && (MSC_Handle->unit[lun].error == MSC_OK))
    {
        res = 1U;
    }
    else
    {
        res = 0U;
    }

    return res;
}

/**
  * @brief  USBH_MSC_GetLUNInfo
  *         The function return a LUN information
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_GetLUNInfo(USBH_HandleTypeDef *phost, uint8_t lun, MSC_LUNTypeDef *info)
{
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;
    if (phost->gState == HOST_CLASS)
    {
        USBH_memcpy(info, &MSC_Handle->unit[lun], sizeof(MSC_LUNTypeDef));
        return USBH_OK;
    }
    else
    {
        return USBH_FAIL;
    }
}

/**
  * @brief  USBH_MSC_Read
  *         The function performs a Read operation
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to read
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_Read(USBH_HandleTypeDef *phost,
                                 uint8_t lun,
                                 uint32_t address,
                                 uint8_t *pbuf,
                                 uint32_t length)
{
    uint32_t timeout;
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    if ((phost->device.is_connected == 0U) ||
            (phost->gState != HOST_CLASS) ||
            (MSC_Handle->unit[lun].state != MSC_IDLE))
    {
        return  USBH_FAIL;
    }

    MSC_Handle->state = MSC_READ;
    MSC_Handle->unit[lun].state = MSC_READ;
    MSC_Handle->rw_lun = lun;

    USBH_MSC_SCSI_Read(phost, lun, address, pbuf, length);

    timeout = phost->Timer;

    while (USBH_MSC_RdWrProcess(phost, lun) == USBH_BUSY)
    {
        if (((phost->Timer - timeout) > (10000U * length)) || (phost->device.is_connected == 0U))
        {
            MSC_Handle->state = MSC_IDLE;
            return USBH_FAIL;
        }
    }
    MSC_Handle->state = MSC_IDLE;

    return USBH_OK;
}

/**
  * @brief  USBH_MSC_Write
  *         The function performs a Write operation
  * @param  phost: Host handle
  * @param  lun: logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to write
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_Write(USBH_HandleTypeDef *phost,
                                  uint8_t lun,
                                  uint32_t address,
                                  uint8_t *pbuf,
                                  uint32_t length)
{
    uint32_t timeout;
    MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

    if ((phost->device.is_connected == 0U) ||
            (phost->gState != HOST_CLASS) ||
            (MSC_Handle->unit[lun].state != MSC_IDLE))
    {
        return  USBH_FAIL;
    }

    MSC_Handle->state = MSC_WRITE;
    MSC_Handle->unit[lun].state = MSC_WRITE;
    MSC_Handle->rw_lun = lun;

    USBH_MSC_SCSI_Write(phost, lun, address, pbuf, length);

    timeout = phost->Timer;
    while (USBH_MSC_RdWrProcess(phost, lun) == USBH_BUSY)
    {
        if (((phost->Timer - timeout) > (10000U * length)) || (phost->device.is_connected == 0U))
        {
            MSC_Handle->state = MSC_IDLE;
            return USBH_FAIL;
        }
    }
    MSC_Handle->state = MSC_IDLE;
    return USBH_OK;
}
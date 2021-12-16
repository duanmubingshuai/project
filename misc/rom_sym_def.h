

#ifndef __ROM_SYM_H__
#define __ROM_SYM_H__
#ifdef USE_ROMSYM_ALIAS



#define __memcpy_fast _symrom___memcpy_fast
#define memcpy _symrom_memcpy
#define memset _symrom_memset
#define memcmp _symrom_memcmp
#define appearanceUUID _symrom_appearanceUUID
#define ATT_CompareUUID _symrom_ATT_CompareUUID
#define ATT_ErrorRsp _symrom_ATT_ErrorRsp
#define ATT_ExchangeMTURsp _symrom_ATT_ExchangeMTURsp
#define ATT_ExecuteWriteRsp _symrom_ATT_ExecuteWriteRsp
#define ATT_FindByTypeValueRsp _symrom_ATT_FindByTypeValueRsp
#define ATT_GetCurrentMTUSize _symrom_ATT_GetCurrentMTUSize
#define ATT_PrepareWriteRsp _symrom_ATT_PrepareWriteRsp
#define ATT_ReadBlobRsp _symrom_ATT_ReadBlobRsp
#define ATT_ReadByGrpTypeRsp _symrom_ATT_ReadByGrpTypeRsp
#define ATT_ReadByTypeRsp _symrom_ATT_ReadByTypeRsp
#define ATT_ReadMultiRsp _symrom_ATT_ReadMultiRsp
#define ATT_ReadRsp _symrom_ATT_ReadRsp
#define ATT_WriteRsp _symrom_ATT_WriteRsp
#define ble_main _symrom_ble_main
#define boot_init _symrom_boot_init
#define characterUUID _symrom_characterUUID
#define charUserDescUUID _symrom_charUserDescUUID
#define clientCharCfgUUID _symrom_clientCharCfgUUID
#define clk_get_pclk _symrom_clk_get_pclk
#define clk_init _symrom_clk_init
#define deviceNameUUID _symrom_deviceNameUUID
#define disableSleep _symrom_disableSleep
#define drv_disable_irq _symrom_drv_disable_irq
#define drv_enable_irq _symrom_drv_enable_irq
#define drv_irq_init _symrom_drv_irq_init
#define enableSleep _symrom_enableSleep
#define g_ATT_MAX_NUM_HANDLES_INFO _symrom_g_ATT_MAX_NUM_HANDLES_INFO
#define g_ATT_MTU_SIZE_MAX _symrom_g_ATT_MTU_SIZE_MAX
#define g_pmCounters _symrom_g_pmCounters
#define g_rfPhyClkSel _symrom_g_rfPhyClkSel
#define g_rfPhyFreqOffSet _symrom_g_rfPhyFreqOffSet
#define g_rfPhyPktFmt _symrom_g_rfPhyPktFmt
#define g_rfPhyTpCal0 _symrom_g_rfPhyTpCal0
#define g_rfPhyTpCal0_2Mbps _symrom_g_rfPhyTpCal0_2Mbps
#define g_rfPhyTpCal1 _symrom_g_rfPhyTpCal1
#define g_rfPhyTpCal1_2Mbps _symrom_g_rfPhyTpCal1_2Mbps
#define g_rfPhyTxPower _symrom_g_rfPhyTxPower
#define g_same_rf_channel_flag _symrom_g_same_rf_channel_flag
#define g_smartWindowSize _symrom_g_smartWindowSize
#define g_system_clk _symrom_g_system_clk
#define GAP_Authenticate _symrom_GAP_Authenticate
#define GAP_CentDevMgrInit _symrom_GAP_CentDevMgrInit
#define GAP_DeviceDiscoveryRequest _symrom_GAP_DeviceDiscoveryRequest
#define GAP_EndDiscoverable _symrom_GAP_EndDiscoverable
#define GAP_GetParamValue _symrom_GAP_GetParamValue
#define GAP_Init _symrom_GAP_Init
#define GAP_MakeDiscoverable _symrom_GAP_MakeDiscoverable
#define GAP_ParamsInit _symrom_GAP_ParamsInit
#define GAP_PasscodeUpdate _symrom_GAP_PasscodeUpdate
#define GAP_PeriDevMgrInit _symrom_GAP_PeriDevMgrInit
#define GAP_ProcessEvent _symrom_GAP_ProcessEvent
#define GAP_RegisterForHCIMsgs _symrom_GAP_RegisterForHCIMsgs
#define GAP_SecParamsInit _symrom_GAP_SecParamsInit
#define GAP_SendSlaveSecurityRequest _symrom_GAP_SendSlaveSecurityRequest
#define GAP_SetParamValue _symrom_GAP_SetParamValue
#define GAP_TerminateAuth _symrom_GAP_TerminateAuth
#define GAP_TerminateLinkReq _symrom_GAP_TerminateLinkReq
#define GAP_UpdateAdvertisingData _symrom_GAP_UpdateAdvertisingData
#define gapServiceUUID _symrom_gapServiceUUID
#define GATT_AppCompletedMsg _symrom_GATT_AppCompletedMsg
#define GATT_FindHandle _symrom_GATT_FindHandle
#define GATT_FindHandleUUID _symrom_GATT_FindHandleUUID
#define GATT_FindNextAttr _symrom_GATT_FindNextAttr
#define GATT_Indication _symrom_GATT_Indication
#define GATT_Init _symrom_GATT_Init
#define GATT_Notification _symrom_GATT_Notification
#define GATT_ProcessEvent _symrom_GATT_ProcessEvent
#define GATT_RegisterForReq _symrom_GATT_RegisterForReq
#define GATT_RegisterService _symrom_GATT_RegisterService
#define GATT_VerifyReadPermissions _symrom_GATT_VerifyReadPermissions
#define gattServiceUUID _symrom_gattServiceUUID
#define gpio_cfg_analog_io _symrom_gpio_cfg_analog_io
#define gpio_debug_mux _symrom_gpio_debug_mux
#define gpio_dir _symrom_gpio_dir
#define gpio_ds_control _symrom_gpio_ds_control
#define gpio_fast_write _symrom_gpio_fast_write
#define gpio_fmux_control _symrom_gpio_fmux_control
#define gpio_fmux_set _symrom_gpio_fmux_set
#define gpio_init _symrom_gpio_init
#define GPIO_IRQHandler _symrom_GPIO_IRQHandler
#define gpio_pin2pin3_control _symrom_gpio_pin2pin3_control
#define gpio_pull_set _symrom_gpio_pull_set
#define gpio_read _symrom_gpio_read
#define gpio_retention _symrom_gpio_retention
#define gpio_wakeup_set _symrom_gpio_wakeup_set
#define gpio_write _symrom_gpio_write
#define gpioin_disable _symrom_gpioin_disable
#define gpioin_enable _symrom_gpioin_enable
#define gpioin_event _symrom_gpioin_event
#define gpioin_init _symrom_gpioin_init
#define gpioin_register _symrom_gpioin_register
#define gpioin_unregister _symrom_gpioin_unregister
#define HCI_Init _symrom_HCI_Init
#define HCI_LE_SetDataLengthCmd _symrom_HCI_LE_SetDataLengthCmd
#define HCI_LE_SetDefaultPhyMode _symrom_HCI_LE_SetDefaultPhyMode
#define HCI_LE_SetPhyMode _symrom_HCI_LE_SetPhyMode
#define HCI_ProcessEvent _symrom_HCI_ProcessEvent
#define HCI_ReadRssiCmd _symrom_HCI_ReadRssiCmd
#define ISR_entry_time _symrom_ISR_entry_time
#define JUMP_FUNCTION_SET _symrom_JUMP_FUNCTION_SET
#define krhino_intrpt_enter_hook _symrom_krhino_intrpt_enter_hook
#define krhino_intrpt_exit_hook _symrom_krhino_intrpt_exit_hook
#define L2CAP_ConnParamUpdateReq _symrom_L2CAP_ConnParamUpdateReq
#define L2CAP_Init _symrom_L2CAP_Init
#define L2CAP_ProcessEvent _symrom_L2CAP_ProcessEvent
#define linkDB_Find _symrom_linkDB_Find
#define linkDB_Register _symrom_linkDB_Register
#define linkDB_State _symrom_linkDB_State
#define ll_hw_clr_irq _symrom_ll_hw_clr_irq
#define ll_hw_get_irq_status _symrom_ll_hw_get_irq_status
#define LL_Init _symrom_LL_Init
#define LL_InitConnectContext _symrom_LL_InitConnectContext
#define LL_PLUS_DisableSlaveLatency _symrom_LL_PLUS_DisableSlaveLatency
#define LL_PLUS_EnableSlaveLatency _symrom_LL_PLUS_EnableSlaveLatency
#define ll_processBasicIRQ _symrom_ll_processBasicIRQ
#define LL_ProcessEvent _symrom_LL_ProcessEvent
#define LL_ReadCarrSens _symrom_LL_ReadCarrSens
#define LL_ReadFoff _symrom_LL_ReadFoff
#define LL_ReadRssi _symrom_LL_ReadRssi
#define llInitFeatureSet2MPHY _symrom_llInitFeatureSet2MPHY
#define llInitFeatureSetDLE _symrom_llInitFeatureSetDLE
#define llSecAdvAllow _symrom_llSecAdvAllow
#define llSecondaryState _symrom_llSecondaryState
#define llSetupSecAdvEvt _symrom_llSetupSecAdvEvt
#define llSetupSecScan _symrom_llSetupSecScan
#define llWaitingIrq _symrom_llWaitingIrq
#define log_printf _symrom_log_printf
#define osal_CbTimerInit _symrom_osal_CbTimerInit
#define osal_CbTimerProcessEvent _symrom_osal_CbTimerProcessEvent
#define osal_CbTimerStart _symrom_osal_CbTimerStart
#define osal_CbTimerStop _symrom_osal_CbTimerStop
#define osal_clear_event _symrom_osal_clear_event
#define osal_get_timeoutEx _symrom_osal_get_timeoutEx
#define osal_init_system _symrom_osal_init_system
#define osal_isbufset _symrom_osal_isbufset
#define osal_mem_alloc _symrom_osal_mem_alloc
#define osal_mem_free _symrom_osal_mem_free
#define osal_mem_set_heap _symrom_osal_mem_set_heap
#define osal_memcmp _symrom_osal_memcmp
#define osal_memcpy _symrom_osal_memcpy
#define osal_memset _symrom_osal_memset
#define osal_msg_allocate _symrom_osal_msg_allocate
#define osal_msg_deallocate _symrom_osal_msg_deallocate
#define osal_msg_receive _symrom_osal_msg_receive
#define osal_msg_send _symrom_osal_msg_send
#define osal_pwrmgr_device _symrom_osal_pwrmgr_device
#define osal_revmemcpy _symrom_osal_revmemcpy
#define osal_set_event _symrom_osal_set_event
#define osal_start_system _symrom_osal_start_system
#define osal_start_timerEx _symrom_osal_start_timerEx
#define osal_stop_timerEx _symrom_osal_stop_timerEx
#define osal_strlen _symrom_osal_strlen
#define osal_sys_tick _symrom_osal_sys_tick
#define osal_pwrmgr_powerconserve0 _symrom_osal_pwrmgr_powerconserve0
#define periConnParamUUID _symrom_periConnParamUUID
#define primaryServiceUUID _symrom_primaryServiceUUID
#define pwrmgr_config _symrom_pwrmgr_config
#define pwrmgr_init _symrom_pwrmgr_init
#define pwrmgr_RAM_retention_set _symrom_pwrmgr_RAM_retention_set
#define pwrmgr_LowCurrentLdo_enable _symrom_pwrmgr_LowCurrentLdo_enable
#define pwrmgr_register _symrom_pwrmgr_register
#define s_config_swClk0 _symrom_s_config_swClk0
#define s_config_swClk1 _symrom_s_config_swClk1
#define pwrmgr_clk_gate_config _symrom_pwrmgr_clk_gate_config
#define read_current_fine_time _symrom_read_current_fine_time
#define rf_phy_ini _symrom_rf_phy_ini
#define rf_phy_set_txPower _symrom_rf_phy_set_txPower
#define rtc_get_counter _symrom_rtc_get_counter
#define s_gpio_wakeup_src _symrom_s_gpio_wakeup_src
#define scanInfo _symrom_scanInfo
#define secondaryServiceUUID _symrom_secondaryServiceUUID
#define serviceChangedUUID _symrom_serviceChangedUUID
#define setSleepMode _symrom_setSleepMode
#define SM_Init _symrom_SM_Init
#define SM_ProcessEvent _symrom_SM_ProcessEvent
#define SM_ResponderInit _symrom_SM_ResponderInit
#define swu_tx_mode _symrom_swu_tx_mode
#define swu_uart_init _symrom_swu_uart_init
#define WaitRTCCount _symrom_WaitRTCCount
#define wakeup_init _symrom_wakeup_init
#define wakeup_init0 _symrom_wakeup_init0
#define watchdog_init _symrom_watchdog_init
#define ll_hw_go0 _symrom_ll_hw_go0
#define rf_calibrate _symrom_rf_calibrate
#define g_dtmAccessCode _symrom_g_dtmAccessCode 
#define g_dtmCarrSens _symrom_g_dtmCarrSens
#define g_dtmCmd _symrom_g_dtmCmd
#define g_dtmCtrl _symrom_g_dtmCtrl
#define g_dtmEvt _symrom_g_dtmEvt
#define g_dtmExtLen _symrom_g_dtmExtLen
#define g_dtmFoff _symrom_g_dtmFoff
#define g_dtmFreq _symrom_g_dtmFreq
#define g_dtmLength _symrom_g_dtmLength
#define g_dtmManualConfig _symrom_g_dtmManualConfig
#define g_dtmModeType _symrom_g_dtmModeType
#define g_dtmPara _symrom_g_dtmPara
#define g_dtmPerAutoIntv _symrom_g_dtmPerAutoIntv
#define g_dtmPKT _symrom_g_dtmPKT
#define g_dtmPktCount _symrom_g_dtmPktCount
#define g_dtmPktIntv _symrom_g_dtmPktIntv
#define g_dtmRsp _symrom_g_dtmRsp
#define g_dtmRssi _symrom_g_dtmRssi
#define g_dtmRxCrcNum _symrom_g_dtmRxCrcNum
#define g_dtmRxTONum _symrom_g_dtmRxTONum
#define g_dtmStatus _symrom_g_dtmStatus
#define g_dtmTick _symrom_g_dtmTick
#define g_dtmTpCalEnable _symrom_g_dtmTpCalEnable
#define g_dtmTxPower _symrom_g_dtmTxPower
#define g_getPn23_cnt _symrom_g_getPn23_cnt
#define g_getPn23_seed _symrom_g_getPn23_seed
#define g_rfPhyDtmCmd _symrom_g_rfPhyDtmCmd
#define g_rfPhyDtmEvt _symrom_g_rfPhyDtmEvt
#define g_rfPhyRxDcIQ _symrom_g_rfPhyRxDcIQ
#define hal_uart_send_byte _symrom_hal_uart_send_byte
#define bit_to_byte _symrom_bit_to_byte
#define ll_hw_write_tfifo _symrom_ll_hw_write_tfifo
#define rf_calibrate _symrom_rf_calibrate
#define rf_calibrate0 _symrom_rf_calibrate0
#define rf_phy_ana_cfg _symrom_rf_phy_ana_cfg
#define rf_phy_bb_cfg _symrom_rf_phy_bb_cfg
#define rf_phy_change_cfg _symrom_rf_phy_change_cfg
#define rf_phy_change_cfg0 _symrom_rf_phy_change_cfg0

#define rf_phy_get_pktFoot_fromPkt _symrom_rf_phy_get_pktFoot_fromPkt
#define rf_phy_get_pktFoot _symrom_rf_phy_get_pktFoot

#define rf_rxDcoc_cfg _symrom_rf_rxDcoc_cfg
#define rf_tp_cal _symrom_rf_tp_cal

#define rf_tpCal_cfg_avg _symrom_rf_tpCal_cfg_avg
#define rf_tpCal_gen_cap_arrary _symrom_rf_tpCal_gen_cap_arrary

#define ll_hw_set_timing _symrom_ll_hw_set_timing
#define ll_hw_rst_tfifo _symrom_ll_hw_rst_tfifo
#define ll_hw_rst_rfifo _symrom_ll_hw_rst_rfifo
#define set_max_length _symrom_set_max_length
#define ll_hw_set_irq _symrom_ll_hw_set_irq
#define ll_hw_set_stx _symrom_ll_hw_set_stx
#define ll_hw_set_srx _symrom_ll_hw_set_srx
#define ll_hw_trigger _symrom_ll_hw_trigger
#define ll_hw_set_trx_settle _symrom_ll_hw_set_trx_settle
#define WaitUs _symrom_WaitUs
#define set_timer _symrom_set_timer
#define ll_hw_get_tr_mode _symrom_ll_hw_get_tr_mode
#define ll_hw_set_trx _symrom_ll_hw_set_trx
#define ll_hw_set_tx_rx_release _symrom_ll_hw_set_tx_rx_release
#define ll_hw_set_rx_tx_interval _symrom_ll_hw_set_rx_tx_interval
#define ll_hw_set_tx_rx_interval _symrom_ll_hw_set_tx_rx_interval
#define ll_hw_set_pplus_pktfmt _symrom_ll_hw_set_pplus_pktfmt
#define ll_hw_ign_rfifo _symrom_ll_hw_ign_rfifo
#define ll_hw_set_crc_fmt _symrom_ll_hw_set_crc_fmt


#define ll_hw_set_rx_timeout _symrom_ll_hw_set_rx_timeout
#define ll_hw_read_rfifo_pplus _symrom_ll_hw_read_rfifo_pplus
#define ll_hw_read_rfifo _symrom_ll_hw_read_rfifo
#define ll_hw_get_tfifo_wrptr _symrom_ll_hw_get_tfifo_wrptr
#define getRxBufferFree _symrom_getRxBufferFree
#define getRxBufferSize _symrom_getRxBufferSize
#define getTxBufferSize _symrom_getTxBufferSize
#define get_tx_read_ptr _symrom_get_tx_read_ptr
#define update_tx_read_ptr _symrom_update_tx_read_ptr
#define conn_param _symrom_conn_param
#define rfCounters _symrom_rfCounters
#define ll_hw_read_tfifo_packet0 _symrom_ll_hw_read_tfifo_packet0
#define ll_hw_get_tfifo_info _symrom_ll_hw_get_tfifo_info
#define g_system_reset_cause _symrom_g_system_reset_cause
#define hal_uart_send_buff _symrom_hal_uart_send_buff
#define log_vsprintf _symrom_log_vsprintf
#define hal_uart_init _symrom_hal_uart_init
#define includeUUID _symrom_includeUUID
#define extReportRefUUID _symrom_extReportRefUUID
#define reportRefUUID _symrom_reportRefUUID
#define osal_start_reload_timer _symrom_osal_start_reload_timer
#define linkDB_PerformFunc _symrom_linkDB_PerformFunc
#endif
#endif


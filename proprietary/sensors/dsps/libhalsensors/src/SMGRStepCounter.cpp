/*============================================================================
  @file SMGRStepCounter.cpp

  @brief
  SMGRStepCounter class implementation.

  Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

#include "SMGRStepCounter.h"

/*============================================================================
  SMGRStepCounter Constructor
============================================================================*/
SMGRStepCounter::SMGRStepCounter(int handle)
    :SMGRSensor(handle),
    smgr_last_step_count(0)
{
    trigger_mode = SENSOR_MODE_EVENT;
    (handle == HANDLE_SMGR_STEP_COUNT_WAKE_UP)?(bWakeUp = true):(bWakeUp = false);
}

/*============================================================================
  SMGRStepCounter Destructor
============================================================================*/
SMGRStepCounter::~SMGRStepCounter()
{

}
/*===========================================================================
  FUNCTION:  setSensorInfo
    Fill the sensor information from the sns_smgr_sensor_datatype_info_s_v01 type
    Parameters
    @datatype : sensor information got from the sensor1 callback
    @info : sensor information to be reported to the framework
===========================================================================*/
void SMGRStepCounter::setSensorInfo(sns_smgr_sensor_datatype_info_s_v01* sensor_datatype)
{
    HAL_LOG_DEBUG("%s: Step Count DTy: %d", __FUNCTION__, sensor_datatype->DataType);
    setType(SENSOR_TYPE_STEP_COUNTER);
    if(bWakeUp == true) {
        strlcat(name, " -Wakeup", sizeof(name));
        setFlags(SENSOR_FLAG_ON_CHANGE_MODE|SENSOR_FLAG_WAKE_UP);
    }
    else {
        setFlags(SENSOR_FLAG_ON_CHANGE_MODE);
    }

    setResolution(1);
    setMaxRange(1);
    setMinFreq(SMGR_SCOUNT_REPORT_RATE_MIN_HZ);
    return;
}

/*===========================================================================
  FUNCTION:  processReportInd
    process the sensor data indicator from the sensor1 type to sensor event type
    Parameters
    @smgr_data : the sensor1 data message from the sensor1 callback
    @sensor_data : the sensor event message that will be send to framework
===========================================================================*/
void SMGRStepCounter::processReportInd(sns_smgr_periodic_report_ind_msg_v01* smgr_ind,
            sns_smgr_data_item_s_v01* smgr_data, sensors_event_t &sensor_data)
{
    UNREFERENCED_PARAMETER(smgr_ind);
    sensor_data.type = SENSOR_TYPE_STEP_COUNTER;
    if(bWakeUp == false) {
        sensor_data.sensor = HANDLE_SMGR_STEP_COUNT;
        HAL_LOG_VERBOSE("%s:sensor %s ",__FUNCTION__,
                    Utility::SensorTypeToSensorString(getType()));
    } else {
        sensor_data.sensor = HANDLE_SMGR_STEP_COUNT_WAKE_UP;
        HAL_LOG_VERBOSE("%s:sensor %s (wake_up)",__FUNCTION__,
                        Utility::SensorTypeToSensorString(getType()));
    }

    if (smgr_last_step_count > (uint32_t)smgr_data->ItemData[0]) {
        sensor_data.u64.step_counter =
            last_event.u64.step_counter + smgr_data->ItemData[0];
    }
    else {
        sensor_data.u64.step_counter =
            last_event.u64.step_counter + smgr_data->ItemData[0] - smgr_last_step_count;
    }

    smgr_last_step_count = smgr_data->ItemData[0];
    HAL_LOG_DEBUG("%s: Step count:%" PRId64 " SMGR Step count:%d smgr_last_step_count:%d",
                  __FUNCTION__, sensor_data.u64.step_counter, smgr_data->ItemData[0],
                  smgr_last_step_count);
}

/*===========================================================================
  FUNCTION:  prepareAddMsg
    SMGRSensor::SMGRPrepareAddMsg will call this function and this func will
    fill the item that needed for this type of sensor.
    Parameters
    @buff_req : the sensor1 message buffer
===========================================================================*/
void SMGRStepCounter::prepareAddMsg(sns_smgr_buffering_req_msg_v01 **buff_req)
{
    (*buff_req)->Item[0].SensorId = SNS_SMGR_ID_STEP_COUNT_V01;
    (*buff_req)->Item[0].Decimation = SNS_SMGR_DECIMATION_RECENT_SAMPLE_V01;
}

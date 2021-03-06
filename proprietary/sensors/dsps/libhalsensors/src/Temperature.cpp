/*============================================================================
  @file Temperature.cpp

  @brief
  Temperature class implementation.

  Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

#include "Temperature.h"

/*============================================================================
  Temperature Constructor
============================================================================*/
Temperature::Temperature(int handle)
    :SMGRSensor(handle)
{
    trigger_mode = SENSOR_MODE_EVENT;
    (handle == HANDLE_AMBIENT_TEMPERATURE_WAKE_UP)?(bWakeUp = true):(bWakeUp = false);
}

/*============================================================================
  Temperature Destructor
============================================================================*/
Temperature::~Temperature()
{

}
/*===========================================================================
  FUNCTION:  setSensorInfo
    Fill the sensor information from the sns_smgr_sensor_datatype_info_s_v01 type
    Parameters
    @datatype : sensor information got from the sensor1 callback
    @info : sensor information to be reported to the framework
===========================================================================*/
void Temperature::setSensorInfo(sns_smgr_sensor_datatype_info_s_v01* sensor_datatype)
{
    HAL_LOG_DEBUG("%s: AMBIENT_TEMPERATURE DTy: %d", __FUNCTION__, sensor_datatype->DataType);
    setType(SENSOR_TYPE_AMBIENT_TEMPERATURE);
    if(bWakeUp == false) {
        setFlags(SENSOR_FLAG_ON_CHANGE_MODE);
     } else {
        strlcat(name, " -Wakeup", sizeof(name));
        setFlags(SENSOR_FLAG_ON_CHANGE_MODE|SENSOR_FLAG_WAKE_UP);
     }

    setResolution((float)((float)sensor_datatype->Resolution *
            UNIT_CONVERT_TEMPERATURE));
    setMaxRange((float)((float)sensor_datatype->MaxRange *
            UNIT_CONVERT_TEMPERATURE));
    return;
}

/*===========================================================================
  FUNCTION:  processReportInd
    process the sensor data indicator from the sensor1 type to sensor event type
    Parameters
    @smgr_data : the sensor1 data message from the sensor1 callback
    @sensor_data : the sensor event message that will be send to framework
===========================================================================*/
void Temperature::processReportInd(sns_smgr_periodic_report_ind_msg_v01* smgr_ind,
            sns_smgr_data_item_s_v01* smgr_data, sensors_event_t &sensor_data)
{
    UNREFERENCED_PARAMETER(smgr_ind);
    sensor_data.type = SENSOR_TYPE_AMBIENT_TEMPERATURE;

    if(bWakeUp == false) {
        sensor_data.sensor = HANDLE_AMBIENT_TEMPERATURE;
        HAL_LOG_VERBOSE("%s:sensor %s ",__FUNCTION__,
                    Utility::SensorTypeToSensorString(getType()));
    } else {
        HAL_LOG_VERBOSE("%s:sensor %s (wake_up)",__FUNCTION__,
                    Utility::SensorTypeToSensorString(getType()));
        sensor_data.sensor = HANDLE_AMBIENT_TEMPERATURE_WAKE_UP;
    }

    sensor_data.temperature = (float)(smgr_data->ItemData[0]) * UNIT_CONVERT_TEMPERATURE;
    HAL_LOG_VERBOSE("%s: Tempr: %f", __FUNCTION__, sensor_data.temperature);
}

/*===========================================================================
  FUNCTION:  prepareAddMsg
    SMGRSensor::SMGRPrepareAddMsg will call this function and this func will
    fill the item that needed for this type of sensor.
    Parameters
    @buff_req : the sensor1 message buffer
===========================================================================*/
void Temperature::prepareAddMsg(sns_smgr_buffering_req_msg_v01 **buff_req)
{
    (*buff_req)->Item[0].SensorId = SNS_SMGR_ID_HUMIDITY_V01;
    (*buff_req)->Item[0].DataType = SNS_SMGR_DATA_TYPE_SECONDARY_V01;
}

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_qualcomm_qti_sensors_core_sensortest_SensorThresh */

#ifndef _Included_com_qualcomm_qti_sensors_core_sensortest_SensorThresh
#define _Included_com_qualcomm_qti_sensors_core_sensortest_SensorThresh
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_qualcomm_qti_sensors_core_sensortest_SensorThresh
 * Method:    registerThresholdNative
 * Signature: (IIIFFFLcom/qualcomm/qti/sensors/core/qsensortest/QSensorEventListener;)I
 */
JNIEXPORT jint JNICALL Java_com_qualcomm_qti_sensors_core_sensortest_SensorThresh_registerThresholdNative
  (JNIEnv *, jclass, jint, jint, jint, jfloat, jfloat, jfloat, jobject);

/*
 * Class:     com_qualcomm_qti_sensors_core_sensortest_SensorThresh
 * Method:    unregisterThresholdNative
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_qualcomm_qti_sensors_core_sensortest_SensorThresh_unregisterThresholdNative
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif

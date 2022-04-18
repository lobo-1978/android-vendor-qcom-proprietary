/**
 *Copyright (c) 2017 Qualcomm Technologies, Inc.
 *All Rights Reserved.
 *Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
  {
    .actuator_params =
    {
      .module_name = "onsemi",
      .actuator_name = "dw9800",
      .i2c_addr = 0x18,
      .i2c_freq_mode = SENSOR_I2C_MODE_FAST,
      .i2c_data_type = CAMERA_I2C_WORD_DATA,
      .i2c_addr_type = CAMERA_I2C_BYTE_ADDR,
      .act_type = ACTUATOR_TYPE_BIVCM,
      .data_size = 10,
      .reg_tbl =
      {
        .reg_tbl_size = 1,
        .reg_params =
        {
          {
            .reg_write_type = ACTUATOR_WRITE_DAC,
            .hw_mask = 0x00000000,
            .reg_addr = 0x03,
            .hw_shift = 0,
            .data_shift = 0,
          },
        },
      },
      //.init_setting_size = 22,
      .init_setting_size = 3,
      .init_settings =
      {
        { 0x02, CAMERA_I2C_BYTE_ADDR, 0x02, CAMERA_I2C_BYTE_DATA, ACTUATOR_I2C_OP_WRITE, 0 },
        { 0x06, CAMERA_I2C_BYTE_ADDR, 0x40, CAMERA_I2C_BYTE_DATA, ACTUATOR_I2C_OP_WRITE, 0 },
        { 0x07, CAMERA_I2C_BYTE_ADDR, 0x7a, CAMERA_I2C_BYTE_DATA, ACTUATOR_I2C_OP_WRITE, 0 },
      },
    }, /* actuator_params */

    .actuator_tuned_params =
    {
      .scenario_size =
      {
        1, /* MOVE_NEAR */
        1, /* MOVE_FAR */
      },
      .ringing_scenario =
      {
        /* MOVE_NEAR */
        {
          336,
        },
        /* MOVE_FAR */
        {
          336,
        },
      },
      .initial_code = 150,//180,
      .region_size = 1,
      .region_params =
      {
        {
          .step_bound =
          {
            400,//336, /* Macro step boundary*/
            0, /* Infinity step boundary*/
          },
          .code_per_step = 1,
          .qvalue = 128,
        },
      },
      .damping =
      {
        /* damping[MOVE_NEAR] */
        {
          /* Scenario 0 */
          {
            .ringing_params =
            {
              /* Region 0 */
              {
                .damping_step = 0xFFF,
                .damping_delay = 1000,
                .hw_params = 0x0000180,
              },
            },
          },
        },
        /* damping[MOVE_NEAR] */
        {
          /* Scenario 0 */
          {
            .ringing_params =
            {
              /* Region 0 */
              {
                .damping_step = 0xFFF,
                .damping_delay = 1000,
                .hw_params = 0x0000FE80,
              },
            },
          },
        },
      },
    }, /* actuator_tuned_params */
  },

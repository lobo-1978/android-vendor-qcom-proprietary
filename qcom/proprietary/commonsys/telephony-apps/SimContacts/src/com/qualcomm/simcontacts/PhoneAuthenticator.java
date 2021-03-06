/*
 * Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 *
 * Not a Contribution, Apache license notifications and license are retained
 * for attribution purposes only.
 */

/*
 * Copyright (C) 2009 The Android Open Source Project
 * Copyright (C) 2012, The Code Aurora Forum. All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.qualcomm.simcontacts;

import android.accounts.AbstractAccountAuthenticator;
import android.accounts.Account;
import android.accounts.AccountAuthenticatorResponse;
import android.accounts.AccountManager;
import android.accounts.NetworkErrorException;
import android.app.admin.DevicePolicyManager;
import android.content.ContentResolver;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;

import java.util.Arrays;

public class PhoneAuthenticator extends AbstractAccountAuthenticator {
    private final String TAG = "PhoneAuthenticator";
    private final Context mContext;
    private final AccountManager mAccountManager;

    public PhoneAuthenticator(Context context) {
        super(context);
        mContext = context;
        mAccountManager = AccountManager.get(mContext);
    }

    @Override
    public Bundle addAccount(AccountAuthenticatorResponse response,
            String accountType, String authTokenType,
            String[] requiredFeatures, Bundle options)
            throws NetworkErrorException {
        Log.d(TAG, "Add phone account");
        final Account account = new Account(SimContactsConstants.ACCOUNT_NAME_PHONE,
                SimContactsConstants.ACCOUNT_TYPE_PHONE);
        mAccountManager.addAccountExplicitly(account, "", null);
        ContentResolver.setIsSyncable(account, "com.android.contacts", 1);
        return null;
    }

    @Override
    public Bundle confirmCredentials(AccountAuthenticatorResponse response,
            Account account, Bundle options) throws NetworkErrorException {
        Log.d(TAG, "confirmCredentials");
        return null;
    }

    @Override
    public Bundle editProperties(AccountAuthenticatorResponse response,
            String accountType) {
        Log.d(TAG, "editProperties");
        throw new UnsupportedOperationException();
    }

    @Override
    public Bundle getAuthToken(AccountAuthenticatorResponse response,
            Account account, String authTokenType, Bundle options)
            throws NetworkErrorException {
        Log.d(TAG, "getAuthToken");
        return null;
    }

    @Override
    public String getAuthTokenLabel(String authTokenType) {
        Log.d(TAG, "getAuthTokenLabel");
        return null;
    }

    @Override
    public Bundle hasFeatures(AccountAuthenticatorResponse response,
            Account account, String[] features) throws NetworkErrorException {
        boolean rsp = false;

        if (features != null && features.length > 0) {
            Log.d(TAG, "hasFeatures: " + Arrays.toString(features));

            for (String feature : features) {
                if(feature.equals(
                    DevicePolicyManager.ACCOUNT_FEATURE_DEVICE_OR_PROFILE_OWNER_ALLOWED)){
                    rsp = true;
                    break;
                }
            }
        }
        Log.d(TAG, "hasFeatures return: " + rsp);

        Bundle result = new Bundle();
        result.putBoolean(AccountManager.KEY_BOOLEAN_RESULT, rsp);
        return result;
    }

    @Override
    public Bundle updateCredentials(AccountAuthenticatorResponse response,
            Account account, String authTokenType, Bundle options)
            throws NetworkErrorException {
        Log.d(TAG, "updateCredentials");
        return null;
    }

}

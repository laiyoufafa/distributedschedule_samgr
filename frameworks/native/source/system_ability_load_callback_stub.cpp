/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "system_ability_load_callback_stub.h"

#include "ipc_skeleton.h"
#include "sam_log.h"
#include "system_ability_definition.h"

namespace OHOS {
int32_t SystemAbilityLoadCallbackStub::OnRemoteRequest(uint32_t code,
    MessageParcel& data, MessageParcel& reply, MessageOption& option)
{
    HILOGI("SystemAbilityLoadCallbackStub::OnRemoteRequest, code = %{public}u", code);
    if (!EnforceInterceToken(data)) {
        HILOGW("SystemAbilityLoadCallbackStub::OnRemoteRequest check interface token failed!");
        return ERR_PERMISSION_DENIED;
    }
    switch (code) {
        case ON_LOAD_SYSTEM_ABILITY_SUCCESS:
            return OnLoadSystemAbilitySuccessInner(data, reply);
        case ON_LOAD_SYSTEM_ABILITY_FAIL:
            return OnLoadSystemAbilityFailInner(data, reply);
        default:
            HILOGW("SystemAbilityLoadCallbackStub::OnRemoteRequest unknown request code!");
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
}

int32_t SystemAbilityLoadCallbackStub::OnLoadSystemAbilitySuccessInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = data.ReadInt32();
    if (!CheckInputSystemAbilityId(systemAbilityId)) {
        HILOGW("OnLoadSystemAbilitySuccessInner invalid systemAbilityId:%{public}d !", systemAbilityId);
        return ERR_INVALID_VALUE;
    }
    sptr<IRemoteObject> remoteObject = data.ReadRemoteObject();
    OnLoadSystemAbilitySuccess(systemAbilityId, remoteObject);
    return ERR_NONE;
}

int32_t SystemAbilityLoadCallbackStub::OnLoadSystemAbilityFailInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t systemAbilityId = data.ReadInt32();
    if (!CheckInputSystemAbilityId(systemAbilityId)) {
        HILOGW("OnLoadSystemAbilityFailInner invalid systemAbilityId:%{public}d !", systemAbilityId);
        return ERR_INVALID_VALUE;
    }
    OnLoadSystemAbilityFail(systemAbilityId);
    return ERR_NONE;
}

bool SystemAbilityLoadCallbackStub::CheckInputSystemAbilityId(int32_t systemAbilityId)
{
    return (systemAbilityId >= FIRST_SYS_ABILITY_ID) && (systemAbilityId <= LAST_SYS_ABILITY_ID);
}

bool SystemAbilityLoadCallbackStub::EnforceInterceToken(MessageParcel& data)
{
    std::u16string interfaceToken = data.ReadInterfaceToken();
    return interfaceToken == GetDescriptor();
}
}

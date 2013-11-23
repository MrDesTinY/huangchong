/*
 * Copyright 2010 Ionut Dediu, deionut@yahoo.com
 *
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

#ifndef __USER_SETTINGS_H__
#define __USER_SETTINGS_H__

typedef enum ConnectionType {
    CONN_UDP = 0,
    CONN_BLUETOOTH = 1,
    CONN_COMM = 2
} ConnectionType;

class QString;

class UserSettings
{
    friend class MainWindow;
public:
    UserSettings();
    UserSettings(const UserSettings& settings);
    UserSettings& operator=(const UserSettings& settings);
    virtual ~UserSettings();
    bool operator==(const UserSettings& rhs) const;
    bool operator!=(const UserSettings& rhs) const;

    // Data:
    ConnectionType connectionType;
    int udpPort;
    QString commPort;

private:
    static void LoadSettings(UserSettings* settings);
    static void SaveSettings(const UserSettings* settings);
};
#endif//__USER_SETTINGS_H__

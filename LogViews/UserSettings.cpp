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

#include <QSettings.h>

#include "UserSettings.h"

#define ORGANIZATION_NAME "deion"
#define APP_NAME "LogViews"

#define CONN_TYPE_KEY "CONN_TYPE_KEY"
#define UDP_PORT_KEY "UDP_PORT_KEY"
#define COMM_PORT_KEY "COMM_PORT_KEY"

#define DEFAULT_UDP_PORT 514
#define DEFAULT_COMM_PORT "COM1"

// Constructor, loads with default user settings
UserSettings::UserSettings():
    connectionType(CONN_UDP),
    udpPort(DEFAULT_UDP_PORT),
    commPort(DEFAULT_COMM_PORT)
{
}

UserSettings::UserSettings(const UserSettings& settings):
    connectionType(settings.connectionType),
    udpPort(settings.udpPort),
    commPort(settings.commPort)
{
}

bool UserSettings::operator==(const UserSettings& rhs) const
{
    return
        (connectionType == rhs.connectionType) &&
        (udpPort == rhs.udpPort) &&
        (commPort == rhs.commPort);
}

bool UserSettings::operator!=(const UserSettings& rhs) const
{
        return !(*this == rhs);
}

UserSettings& UserSettings::operator=(const UserSettings& settings)
{
    if(this != &settings)
    {
        connectionType = settings.connectionType;
        udpPort = settings.udpPort;
        commPort = settings.commPort;
    }
    return *this;
}

UserSettings::~UserSettings()
{
}

void UserSettings::LoadSettings(UserSettings* settings)
{
    QSettings qSett(ORGANIZATION_NAME, APP_NAME);

    settings->connectionType = (ConnectionType)qSett.value(CONN_TYPE_KEY, settings->connectionType).toInt();
    settings->udpPort = qSett.value(UDP_PORT_KEY, settings->udpPort).toInt();
    settings->commPort = qSett.value(COMM_PORT_KEY, settings->commPort).toString();
}

void UserSettings::SaveSettings(const UserSettings* settings)
{
    QSettings qSett(ORGANIZATION_NAME, APP_NAME);
    qSett.setValue(CONN_TYPE_KEY, (int)(settings->connectionType));
    qSett.setValue(UDP_PORT_KEY, settings->udpPort);
    qSett.setValue(COMM_PORT_KEY, settings->commPort);
}

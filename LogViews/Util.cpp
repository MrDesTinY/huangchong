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

#include "Util.h"

QString Util::color2RRGGBB(const QColor& color)
{
    QString result;
    int red = color.red();
    int green = color.green();
    int blue = color.blue();

    QString hexRRStr;
    hexRRStr.sprintf("%02X", red);
    QString hexGGStr;
    hexGGStr.sprintf("%02X", green);
    QString hexBBStr;
    hexBBStr.sprintf("%02X", blue);

    result = "#" + hexRRStr + hexGGStr + hexBBStr;
    return result;
}

/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "dtypecolor.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

DTypeColor::DTypeColor()
{
}

QString DTypeColor::colorCode() const
{
    return m_colorCode;
}

void DTypeColor::setColorCode(const QString &colorCode)
{
    m_colorCode = colorCode;
}

int DTypeColor::colorID() const
{
    return m_colorID;
}

void DTypeColor::setColorID(int colorID)
{
    m_colorID = colorID;
}

DTypeColor::Privilege DTypeColor::privilege() const
{
    return m_privilege;
}

void DTypeColor::setPrivilege(const Privilege &privilege)
{
    m_privilege = privilege;
}

bool DTypeColor::isSysColorInfo()
{
}

DTypeColor::List DTypeColor::fromJsonString(const QString &colorJson)
{
    DTypeColor::List colorList;
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(colorJson.toLocal8Bit(), &jsonError));
    if (jsonError.error != QJsonParseError::NoError) {
        qWarning() << "error:" << jsonError.errorString();
        return colorList;
    }
    QJsonArray rootArr = jsonDoc.array();
    foreach (auto json, rootArr) {
        QJsonObject colorObj = json.toObject();
        DTypeColor::Ptr typeColor = DTypeColor::Ptr(new DTypeColor);
        if (colorObj.contains("colorID")) {
            typeColor->setColorID(colorObj.value("colorID").toInt());
        }
        if (colorObj.contains("colorCode")) {
            typeColor->setColorCode(colorObj.value("colorCode").toString());
        }
        if (colorObj.contains("privilege")) {
            typeColor->setPrivilege(static_cast<Privilege>(colorObj.value("privilege").toInt()));
        }
        colorList.append(typeColor);
    }
    return colorList;
}

QString DTypeColor::toJsonString(const DTypeColor::List &colorList)
{
    QJsonArray rootArr;
    foreach (auto color, colorList) {
        QJsonObject colorObj;
        colorObj.insert("colorID", color->colorID());
        colorObj.insert("colorCode", color->colorCode());
        colorObj.insert("privilege", color->privilege());
        rootArr.append(colorObj);
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(rootArr);
    return QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
}
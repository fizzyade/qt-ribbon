/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
 *
 * Created by Adrian Carpenter on 12/12/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RibbonButtonPlugin.h"

#include "RibbonButton.h"

#include <QRegularExpression>
#include <QtPlugin>

constexpr auto ConfigurationXML = R"(
    <ui language="c++" displayname="Ribbon Button">
        <widget class="Nedrysoft::Ribbon::RibbonButton" name="ribbonButton">
            <property name="text">
                <string>Button</string>
            </property>
        </widget>
    </ui>
)";

RibbonButtonPlugin::RibbonButtonPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {

}

void RibbonButtonPlugin::initialize(QDesignerFormEditorInterface *core) {
    Q_UNUSED(core)

    if (m_initialized) {
        return;
    }

    m_initialized = true;
}

bool RibbonButtonPlugin::isInitialized() const {
    return m_initialized;
}

QWidget *RibbonButtonPlugin::createWidget(QWidget *parent) {
    return new Nedrysoft::Ribbon::RibbonButton(parent);
}

QString RibbonButtonPlugin::name() const {
    return QStringLiteral("Nedrysoft::Ribbon::RibbonButton");
}

QString RibbonButtonPlugin::group() const {
    return QStringLiteral("Nedrysoft Ribbon Widgets");
}

QIcon RibbonButtonPlugin::icon() const {
    return QIcon(":/Nedrysoft/Ribbon/icons/ribbon.png");
}

QString RibbonButtonPlugin::toolTip() const {
    return tr("A Ribbon Button");
}

QString RibbonButtonPlugin::whatsThis() const {
    return tr("The Ribbon Button is a control that provides a push button and is styled to match the Ribbon.");
}

bool RibbonButtonPlugin::isContainer() const {
    return true;
}

QString RibbonButtonPlugin::domXml() const {
    return ConfigurationXML;
}

QString RibbonButtonPlugin::includeFile() const {
    return QStringLiteral("<RibbonButton>");
}

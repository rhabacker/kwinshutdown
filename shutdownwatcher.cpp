/*
    Copyright 2016 ralf.habacker <ralf.habacker@freenet.de>
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "shutdownwatcher.h"
#include "main.h"

#include <KDebug>

#include <QApplication>
#include <QStringList>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QtDebug>

ShutdownWatcher::ShutdownWatcher(const QString &appName, int timeout, QObject *parent)
  : QObject(parent),
    m_appName(appName),
    m_checkLevel(0),
    m_timeout(timeout)
{
    startWatch();
}

void ShutdownWatcher::startWatch()
{
    int timeout = m_timeout*1000/2;
    kDebug() << "starting timer with timeout" << timeout << "ms";
    QTimer::singleShot(timeout, this, SLOT(slotCheckShutDown()));
}

QStringList ShutdownWatcher::allServiceNamesFromDBus() const
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    QDBusConnectionInterface *bus = connection.interface();
    return bus->registeredServiceNames();
}

void ShutdownWatcher::slotCheckShutDown()
{
    kDebug() << "timeout: checking at level" << m_checkLevel;
    foreach(const QString &app, allServiceNamesFromDBus())
    {
        kDebug() << "checking" << app;
        if (m_checkLevel == 0)
        {
            if (app.contains(m_appName))
            {
                kDebug() << app << "found";
                startWatch();
                return;
            }
        }
        else if (m_checkLevel == 1)
        {
            if (app.contains(m_appName))
            {
                kDebug() << app << "found at level 1";
                m_checkLevel = 0;
                startWatch();
                return;
            }
        }
    }
    if (m_checkLevel == 0)
    {
        m_checkLevel = 1;
        kDebug() << m_appName << "not found, increasing check level to" << m_checkLevel;
        startWatch();
    }
    else if (m_checkLevel == 1)
    {
        kDebug() << m_appName << "not running after two iterations, shutting down KDE";
        shutDownApps();
        qApp->quit();
    }
}


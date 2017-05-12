/* This file is part of the KDE Project

   Copyright (C) 2011 Ralf Habacker <ralf.habacker@freenet.de>
   All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License version 2
   as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "shutdownwatcher.h"

#include <kdebug.h>
#include <KAboutData>
#include <KApplication>
#include <KMessageBox>
#include <KCmdLineArgs>
#include <KGlobal>
#include <KProcess>
#include <KDebug>

#include <windows.h>

static bool dryrun = false;

void hideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

bool requestForShutDownApps()
{
    int res = KMessageBox::warningContinueCancel(0,
    ki18n("Should I really shutdown all applications and processes of your recent installation ?"
    "\n\nPlease make sure you have saved all documents.").toString(),
    ki18n("Shutdown KDE").toString());
    if (res == KMessageBox::Cancel)
        return false;
    return true;
}

int shutDownApps()
{
    QStringList cmd;
    cmd  << "kdeinit4" << "--shutdown";
    kDebug() << "running" << cmd;
    if (!dryrun)
        KProcess::startDetached(cmd);
    return 0;
}

int main(int argc, char **argv)
{
    KAboutData about("kwinshutdown", 0, ki18n("kwinshutdown"), "1.0",
                     ki18n("A helper tool to shutdown a running installation"),
                     KAboutData::License_GPL,
                     ki18n("(C) 2011-2017 Ralf Habacker"));
    KCmdLineArgs::init( argc, argv, &about);

    KCmdLineOptions options;
    options.add("timeout <seconds>", ki18n("Timeout to shut down KDE background processes after a watched application has been exited, defaulting to 60 seconds"), "60");
    options.add("watch <appname>", ki18n("Enable shut down watching for <appname"), "");
    options.add("dry-run", ki18n("Test mode, do not really shut down"), "");
    options.add("hide-console", ki18n("Hide console window"), "");
    KCmdLineArgs::addCmdLineOptions( options ); // Add my own options.

    KComponentData a(&about);

    // Get application specific arguments
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->isSet("hide-console"))
        hideConsole();

    KApplication app(true);

    dryrun = args->isSet("dry-run");

    if (args->isSet("watch"))
    {
        int timeout = args->getOption("timeout").toInt();
        ShutdownWatcher watcher(args->getOption("watch"), timeout);
        return app.exec();
    }
    else
    {
        if (requestForShutDownApps())
            return shutDownApps();
        return 1;
    }
}

// vim: ts=4 sw=4 et

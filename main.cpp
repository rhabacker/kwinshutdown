/* This file is part of the KDE Project

   Copyright (C) 2011-2021 Ralf Habacker <ralf.habacker@freenet.de>
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

#include <KAboutData>
#include <KMessageBox>
#include <KLocalizedString>
#include <KProcess>

#include <QApplication>
#include <QCommandLineParser>
#include <QtDebug>

#include <windows.h>

static bool dryrun = false;

void hideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

bool requestForShutDownApps()
{
    int res = KMessageBox::warningContinueCancel(0,
    i18n("Should I really shutdown all applications and processes of your recent installation ?"
    "\n\nPlease make sure you have saved all documents."),
    i18n("Shutdown KDE"));
    if (res == KMessageBox::Cancel)
        return false;
    return true;
}

int shutDownApps()
{
    QStringList cmd;
    cmd  << "kdeinit5" << "--shutdown";
    qCDebug(KWINSHUTDOWN) << "running" << cmd;
    if (!dryrun)
        KProcess::startDetached(cmd);
    return 0;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kwinshutdown");

    KAboutData aboutData(
                QStringLiteral("kwinshutdown"),
                i18n("KWinShutDown"),
                QStringLiteral("1.0"),
                i18n("A helper tool to shutdown a running installation"),
                KAboutLicense::GPL,
                i18n("(C) 2011-2021 Ralf Habacker")
    );

    aboutData.addAuthor(
                i18n("Ralf Habacker"),
                i18n("Author"),
                QStringLiteral("ralf.habacker@freenet.de")
    );

    QCommandLineParser args;
    aboutData.setupCommandLine(&args);

    QCommandLineOption timeoutOption("timeout",
        i18n("Timeout to shut down KDE background processes after a watched application has been exited, defaulting to 60 seconds"),
        i18n("seconds")
    );
    timeoutOption.setDefaultValue(QStringLiteral("60"));
    args.addOption(timeoutOption);

    QCommandLineOption watchOption("watch", i18n("Enable shut down watching for <appname"), i18n("appname"));
    args.addOption(watchOption);

    QCommandLineOption dryRunOption("dry-run", i18n("Test mode, do not really shut down"));
    args.addOption(dryRunOption);

    QCommandLineOption hideConsoleOption("hide-console", i18n("Hide console window"));
    args.addOption(hideConsoleOption);

    args.process(app);
    aboutData.processCommandLine(&args);

    if (args.isSet(hideConsoleOption))
        hideConsole();

    dryrun = args.isSet(dryRunOption);

    if (args.isSet(watchOption))
    {
        int timeout = args.value(timeoutOption).toInt();
        ShutdownWatcher watcher(args.value(watchOption), timeout);
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

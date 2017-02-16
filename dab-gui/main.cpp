/*
    *    Copyright (C) 2010, 2011, 2012
    *    Jan van Katwijk (J.vanKatwijk@gmail.com)
    *    Lazy Chair Programming
    *
    *    This file is part of the SDR-J.
    *    Many of the ideas as implemented in SDR-J are derived from
    *    other work, made available through the GNU general Public License.
    *    All copyrights of the original authors are recognized.
    *
    *    SDR-J is free software; you can redistribute it and/or modify
    *    it under the terms of the GNU General Public License as published by
    *    the Free Software Foundation; either version 2 of the License, or
    *    (at your option) any later version.
    *
    *    SDR-J is distributed in the hope that it will be useful,
    *    but WITHOUT ANY WARRANTY; without even the implied warranty of
    *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    *    GNU General Public License for more details.
    *
    *    You should have received a copy of the GNU General Public License
    *    along with SDR-J; if not, write to the Free Software
    *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    *
    *	Main program
    */
#include	<QApplication>
#include	<QSettings>
#include	<QDir>
#ifdef	GUI_3
#include	<QCommandLineParser>
#endif
#include	<unistd.h>
#include	"dab-constants.h"
#include	"gui.h"

QString	fullPathfor (QString v) {
    int16_t	i;
    QString	fileName;

    if (v == QString (""))
        return QString ("/tmp/xxx");

    if (v. at (0) == QChar ('/')) 		// full path specified
        return v;

    fileName = QDir::homePath ();
    fileName. append ("/");
    fileName. append (v);
    fileName = QDir::toNativeSeparators (fileName);

    if (!fileName. endsWith (".ini"))
        fileName. append (".ini");

    return fileName;
}

#define	DEFAULT_INI	".welle.io.ini"

int	main (int argc, char **argv) {
    RadioInterface *MyRadioInterface;

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Default values
    uint8_t		syncMethod	= 2;
    QSettings	*dabSettings;		// ini file
    uint8_t		dabMode		= 127;	// illegal value
    QString		dabDevice	= QString ("");
    QString		dabBand		= QString ("");
    QString		ipAddress	= QString ("");
    uint16_t	ipPort		= 1234;

    QString initFileName = fullPathfor (QString (DEFAULT_INI));

    dabSettings =  new QSettings (initFileName, QSettings::IniFormat);
    if (dabMode == 127)
        dabMode = dabSettings -> value ("dabMode", 1). toInt ();
    if (dabDevice == QString (""))
        dabDevice = dabSettings -> value ("device", "dabstick"). toString ();
    if (dabBand == QString (""))
        dabBand = dabSettings -> value ("band", "BAND III"). toString ();

    dabSettings -> beginGroup ("rtl_tcp_client");
    if (ipAddress != QString ("")) {
        dabSettings -> setValue ("rtl_tcp_address", ipAddress);
        dabSettings -> setValue ("rtl_tcp_port", ipPort);
    }
    dabSettings -> endGroup ();

    (void)syncMethod;
    dabSettings -> setValue ("dabMode",	dabMode);
    dabSettings -> setValue ("device",	dabDevice);
    dabSettings -> setValue ("band",	dabBand);
    dabSettings	-> sync ();

    dabSettings	-> endGroup ();
    MyRadioInterface = new RadioInterface (dabSettings,
                               dabDevice,
                               dabMode,
                               dabBand);

    engine.rootContext()->setContextProperty("cppGUI", MyRadioInterface);
    engine.load(QUrl(QStringLiteral("qrc:/dab_gui.qml")));
    engine.addImageProvider(QLatin1String("motslideshow"), MyRadioInterface->MOTImage);
    app.exec ();
    /*
     *	done:
     */
    fflush (stdout);
    fflush (stderr);
    qDebug ("It is done\n");
    MyRadioInterface	-> ~RadioInterface ();
    dabSettings		-> ~QSettings ();
    exit (1);
}


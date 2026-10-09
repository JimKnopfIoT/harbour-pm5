TARGET = harbour-pm5
DEFINES += "APP_TARGET=\\\"$${TARGET}\\\""

CONFIG += sailfishapp
CONFIG += sailfishapp_i18n

QT += network

HEADERS += src/proxmark.h src/pmproxy.h
SOURCES += src/harbour-pm5.cpp src/proxmark.cpp src/pmproxy.cpp

DISTFILES += \
    qml/main.qml \
    qml/cover/CoverPage.qml \
    qml/components/SpaceBackground.qml \
    qml/components/ActionTile.qml \
    qml/components/StatusCard.qml \
    qml/components/DataField.qml \
    qml/components/InfoBlock.qml \
    qml/components/Glossary.qml \
    qml/components/TermList.qml \
    qml/components/LfReference.qml \
    qml/components/MifareReference.qml \
    qml/components/LogView.qml \
    qml/components/ExportDialog.qml \
    qml/components/PinDialog.qml \
    qml/pages/MainPage.qml \
    qml/pages/LfPage.qml \
    qml/pages/LfInfoPage.qml \
    qml/pages/HfPage.qml \
    qml/pages/HfInfoPage.qml \
    qml/pages/MifarePage.qml \
    qml/pages/MifaInfoPage.qml \
    qml/pages/LogPage.qml \
    qml/pages/TerminalPage.qml \
    qml/pages/EmulatePage.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

TRANSLATIONS += translations/harbour-pm5-de.ts

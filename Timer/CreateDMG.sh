#!/bin/bash
if [ $# -eq 0 ]
  then
    echo "No arguments supplied" > /dev/stderr
    exit 1
fi

appName=$1
appFolder=$appName.app
if [ ! -d $appFolder ]
  then
    echo "No folder $appFolder" > /dev/stderr
    exit 2
fi

audioFile=audio.mp3
if [ ! -f $audioFile ]
  then
    echo "No audio file $audioFile" > /dev/stderr
    exit 3
fi

docFile=Timer_Manual.doc
if [ ! -f $docFile ]
  then
    echo "No doc file $docFile" > /dev/stderr
    exit 4
fi

textFolder=TEXTFILES
if [ ! -d $textFolder  ]
  then
    echo "No folder $textFolder" > /dev/stderr
    exit 5
fi

echo "Copy all data to temp folder"
mkdir temp
cp -r $appFolder temp
cp -r $textFolder temp
cp $docFile temp
cp $audioFile temp

echo "Update Qt libs and plugins"
macdeployqt temp/$appFolder

echo "Create dmg file"
DMG=$appName.dmg
rm $DMG
hdiutil create $DMG -volname "$appName" -fs HFS+ -srcfolder "temp"

echo "All done"
exit

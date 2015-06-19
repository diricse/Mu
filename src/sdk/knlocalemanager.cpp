/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QTranslator>

#include <QApplication>

#include "knlocalemanager.h"

#include <QDebug>

KNLocaleManager *KNLocaleManager::m_instance=nullptr;

KNLocaleManager *KNLocaleManager::instance()
{
    return m_instance;
}

KNLocaleManager::KNLocaleManager(QObject *parent) :
    QObject(parent),
    m_translator(new QTranslator(this)),
    m_noLanguageIcon(QPixmap("://public/noIcon.png")),
    m_currentLangauge(-1)
{
    //Check the instance first.
    if(m_instance==nullptr)
    {
        m_instance=this;
    }
}

void KNLocaleManager::loadLanguageFiles(const QString &languageDir)
{
    //Clear the previous language list.
    m_languageList.clear();
    //Load the language translations.
    //This file stored all the language name and it's translation to their
    //native language.
    QFile languageTranslationFile("://public/language.json");
    if(languageTranslationFile.open(QIODevice::ReadOnly))
    {
        //Load the json object from the file.
        QJsonObject languageTranslation=
                QJsonDocument::fromJson(
                    languageTranslationFile.readAll()).object();
        //Close the file.
        languageTranslationFile.close();
        //Load the pair one by one.
        for(QJsonObject::iterator i=languageTranslation.begin();
            i!=languageTranslation.end();
            ++i)
        {
            //Insert the translation to hash list.
            m_languageTranslation.insert(i.key(), i.value().toString());
        }
    }
    //Add English language item, English will always be the first language, it's
    //the default embedded language.
    addLanguage("English", "English", "", QPixmap("://public/English.png"));
    //Load the language in application default folder.
    loadLanguageInFolder(qApp->applicationDirPath()+"/Language");
    //Load the language in the language folder.
    loadLanguageInFolder(languageDir);
}

void KNLocaleManager::setLanguage(const QString &key)
{
    //Generate a fake item.
    LanguageItem targetItem;
    targetItem.key=key;
    //Find the item.
    int itemIndex=m_languageList.indexOf(targetItem);
    //If we cannot find the item, ignore the language set request.
    if(itemIndex==-1)
    {
        return;
    }
    //Save the current language index, load the language file.
    m_currentLangauge=itemIndex;
    loadLanguage(m_languageList.at(m_currentLangauge).filePath);
}

void KNLocaleManager::setLanguage(const int &index)
{
    //Check whether the index is inside the range of the language list.
    if(index>-1 && index<m_languageList.size())
    {
        //Save the language index.
        m_currentLangauge=index;
        //Load the language file.
        loadLanguage(m_languageList.at(m_currentLangauge).filePath);
    }
}

void KNLocaleManager::setDefaultLanguage()
{
    //Generate the default locale, get the system data.
    QLocale locale;
    //Set the language accroding to locale.
    switch (locale.country())
    {
    case QLocale::China:
        setLanguage("Simplified_Chinese");
        break;
    case QLocale::HongKong:
    case QLocale::Macau:
    case QLocale::Taiwan:
        setLanguage("Traditional_Chinese");
        break;
    case QLocale::Japan:
        setLanguage("Japanese");
        break;
    case QLocale::France:
        setLanguage("French");
        break;
    case QLocale::Germany:
        setLanguage("German");
        break;
    case QLocale::Italy:
        setLanguage("Italian");
        break;
    case QLocale::Russia:
        setLanguage("Russian");
        break;
    case QLocale::SouthKorea:
        setLanguage("Korean");
        break;
    case QLocale::Spain:
        setLanguage("Spanish");
        break;
    default:
        setLanguage("English");
        break;
    }
}

void KNLocaleManager::loadLanguage(const QString &filePath)
{
    //Remove the current translator.
    if(!m_translator->isEmpty())
    {
        qApp->removeTranslator(m_translator);
    }
    //Load the translate file to translator.
    m_translator->load(filePath);
    //Check the translator, if it's not empty, install the translator.
    if(!m_translator->isEmpty())
    {
        qApp->installTranslator(m_translator);
    }
    //Ask to retranslate.
    emit languageChange();
}

void KNLocaleManager::loadLanguageInFolder(const QString &dirPath)
{
    //Check the folder exist. If no, return.
    QDir languageFolder(dirPath);
    if(!languageFolder.exists())
    {
        return;
    }
    //Get all the folder in the language list.
    QFileInfoList folderList=languageFolder.entryInfoList();
    for(QFileInfoList::iterator i=folderList.begin();
            i!=folderList.end();
            ++i)
    {
        QString key=(*i).fileName();
        //Ignore if the current info is not folder, dot(.) and dot-dot(..).
        if(!(*i).isDir() || key=="." || key=="..")
        {
            continue;
        }
        //The structure of the language directory should be:
        // Language
        // |-Simplified_Chinese             Key of the lagnuage
        // | |-Simplified_Chinese.qm        Translation file (.qm)
        // | |-Simplified_Chinese.png       Translation icon (.png)
        //Get the language file and the icon file.
        QFileInfo qmInfo((*i).absoluteFilePath()+"/"+key+".qm"),
                  iconInfo((*i).absoluteFilePath()+"/"+key+".png");
        //If there's no qm file, then ignore this folder.
        if(qmInfo.exists())
        {
            //Prepare the information of the language.
            //  Name
            //Get the matched name from the translation file.
            QString matchedName=m_languageTranslation.value(key, QString());
            //  Icon
            //Check the icon.
            QPixmap icon;
            if(iconInfo.exists())
            {
                //Try to load the icon.
                icon.load(iconInfo.absoluteFilePath());
                //Check the icon is valid or not.
                if(icon.isNull())
                {
                    icon=m_noLanguageIcon;
                }
            }
            else
            {
                icon=m_noLanguageIcon;
            }
            //Add new language
            addLanguage(key,
                        matchedName.isEmpty()?key:matchedName,
                        qmInfo.absoluteFilePath(),
                        icon);
        }
    }
}

inline void KNLocaleManager::addLanguage(const QString &key,
                                         const QString &name,
                                         const QString &filePath,
                                         const QPixmap &icon)
{
    //Generate the item.
    LanguageItem currentItem;
    currentItem.key=key;
    currentItem.name=name;
    currentItem.filePath=filePath;
    currentItem.icon=icon;
    //Add item to language list if the item is not in the list.
    if(!m_languageList.contains(currentItem))
    {
        m_languageList.append(currentItem);
    }
}

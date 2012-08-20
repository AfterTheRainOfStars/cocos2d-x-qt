/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include <string.h>

#include <vector>
#include <string>
#include <sstream>

#include <QImage>
#include <QPainter>
#include <QFontDatabase>
#include <QFont>

#include <QMap>


#include "CCPlatformMacros.h"
#include "CCImage.h"
#include "CCStdC.h"

#include "CCFileUtils.h"

#define __CC_PLATFORM_IMAGE_CPP__
#include "CCImageCommon_cpp.h"

NS_CC_BEGIN;

QMap<QString, QString> loadedFontMap;

// TODO: Get rid of this function
static int getBitsPerComponent(const QImage::Format fmt)
{
    switch(fmt)
    {
    case QImage::Format_Mono: return 1;
    case QImage::Format_MonoLSB: return 1;
    case QImage::Format_Indexed8: return 8;
    case QImage::Format_RGB32: return 8;
    case QImage::Format_ARGB32: return 8;
    case QImage::Format_ARGB32_Premultiplied: return 8;
    case QImage::Format_RGB16: return 5;
    case QImage::Format_ARGB8565_Premultiplied: return 8;
    case QImage::Format_RGB666: return 6;
    case QImage::Format_ARGB6666_Premultiplied: return 6;
    case QImage::Format_RGB555: return 5;
    case QImage::Format_ARGB8555_Premultiplied: return 5;
    case QImage::Format_RGB888: return 8;
    case QImage::Format_RGB444: return 4;
    case QImage::Format_ARGB4444_Premultiplied: return 4;
    default:
        CCLOGERROR("Unknown QImage::Format %d", fmt);
        return 0;
    }
}

bool CCImage::initWithString(
        const char * pText,
        int nWidth/* = 0*/,
        int nHeight/* = 0*/,
        ETextAlign eAlignMask/* = kAlignCenter*/,
        const char * pFontName/* = nil*/,
        int nSize/* = 0*/)
{
    if (!pText)
    {
        return false;
    }

    QString fontPath(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFontName));

    QString fontFamily = pFontName;
    QString fontStyle = "Normal";

    bool fontLoaded = false;

    // try to load .ttf font first
    if(!fontPath.endsWith(".ttf"))
    {
        QString fontPathTTF(fontPath);
        fontPathTTF.append(".ttf");

        // font already loaded?
        QMap<QString, QString>::iterator fontIter = loadedFontMap.find(fontPathTTF);
        if(fontIter == loadedFontMap.end())
        {
            int fontID = QFontDatabase::addApplicationFont(fontPathTTF);
            if(fontID != -1)
            {
                QStringList familyList = QFontDatabase::applicationFontFamilies(fontID);

                if(familyList.size() > 0)
                    fontFamily = familyList.at(0);

                loadedFontMap.insert(fontPathTTF, fontFamily);
                fontLoaded = true;
            }

            //loadedFontMap.insert(fontPathTTF, fontFamily);
        }
    }

    if(!fontLoaded)
    {
        // font already loaded?
        QMap<QString, QString>::iterator fontIter = loadedFontMap.find(fontPath);
        if(fontIter == loadedFontMap.end())
        {
            int fontID = QFontDatabase::addApplicationFont(fontPath);
            if(fontID != -1)
            {
                QStringList familyList = QFontDatabase::applicationFontFamilies(fontID);

                if(familyList.size() > 0)
                    fontFamily = familyList.at(0);
            }

            loadedFontMap.insert(fontPath, fontFamily);
        }
        else
        {
            fontFamily = fontIter.value();
        }
    }

    QFontDatabase fd;
    QFont f = fd.font(fontFamily, fontStyle, nSize);
    f.setPixelSize(nSize);

    QFontMetrics fm(f);

    if (nWidth)
    {
        m_nWidth = (short)nWidth;
    }
    else
    {
        m_nWidth = fm.width(QString(pText));
    }

    if (nHeight)
    {
        m_nHeight = (short)nHeight;
    }
    else
    {
        m_nHeight = fm.height();
    }

    m_bHasAlpha = true;
    m_bPreMulti = false;
    m_pData = new unsigned char[m_nWidth * m_nHeight * 4];
    memset(m_pData, 0, m_nWidth * m_nHeight * 4);
    m_nBitsPerComponent = 8;

    QImage image(m_pData, m_nWidth, m_nHeight, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    painter.setFont(f);
    painter.setPen(Qt::white);

    int flags = 0;
    switch (eAlignMask)
    {
    case kAlignCenter: // Horizontal center and vertical center.
        flags |= Qt::AlignHCenter;
        flags |= Qt::AlignVCenter;
        break;
    case kAlignTop: // Horizontal center and vertical top.
        flags |= Qt::AlignHCenter;
        flags |= Qt::AlignTop;
        break;
    case kAlignTopRight: // Horizontal right and vertical top.
        flags |= Qt::AlignRight;
        flags |= Qt::AlignTop;
        break;
    case kAlignRight: // Horizontal right and vertical center.
        flags |= Qt::AlignRight;
        flags |= Qt::AlignVCenter;
        break;
    case kAlignBottomRight: // Horizontal right and vertical bottom.
        flags |= Qt::AlignRight;
        flags |= Qt::AlignBottom;
        break;
    case kAlignBottom: // Horizontal center and vertical bottom.
        flags |= Qt::AlignHCenter;
        flags |= Qt::AlignBottom;
        break;
    case kAlignBottomLeft: // Horizontal left and vertical bottom.
        flags |= Qt::AlignLeft;
        flags |= Qt::AlignBottom;
        break;
    case kAlignLeft: // Horizontal left and vertical center.
        flags |= Qt::AlignLeft;
        flags |= Qt::AlignVCenter;
        break;
    case kAlignTopLeft: // Horizontal left and vertical top.
        flags |= Qt::AlignLeft;
        flags |= Qt::AlignTop;
        break;
    }

    painter.drawText(QRect(0, 0, m_nWidth, m_nHeight), flags, QString(pText));
    painter.end();

    return true;
}

bool CCImage::_initWithJpgData(void * data, int nSize)
{
    QImage image;

    bool bRet = image.loadFromData((const uchar*)data, nSize, "JPG");
    if (!bRet)
        return false;

    // TODO: Better/faster conversion
    QImage convertedImg = image.convertToFormat(
                QImage::Format_ARGB32_Premultiplied);
    convertedImg = convertedImg.rgbSwapped();

    m_bPreMulti = true;
    m_bHasAlpha = convertedImg.hasAlphaChannel();
    m_nBitsPerComponent = cocos2d::getBitsPerComponent(convertedImg.format());
    m_nHeight = (short)convertedImg.height();
    m_nWidth = (short)convertedImg.width();
    m_pData  = new unsigned char[convertedImg.byteCount()];
    memcpy(m_pData, convertedImg.bits(), convertedImg.byteCount());

    return bRet;
}

bool CCImage::_initWithPngData(void * pData, int nDatalen)
{
    QImage image;

    bool bRet = image.loadFromData((const uchar*)pData, nDatalen, "PNG");
    if (!bRet)
        return false;

    // TODO: Better/faster conversion
    QImage convertedImg = image.convertToFormat(
                QImage::Format_ARGB32_Premultiplied);
    convertedImg = convertedImg.rgbSwapped();

    m_bPreMulti = true;
    m_bHasAlpha = convertedImg.hasAlphaChannel();
    m_nBitsPerComponent = cocos2d::getBitsPerComponent(convertedImg.format());
    m_nHeight = (short)convertedImg.height();
    m_nWidth = (short)convertedImg.width();
    m_pData  = new unsigned char[convertedImg.byteCount()];
    memcpy(m_pData, convertedImg.bits(), convertedImg.byteCount());

    return bRet;
}

bool CCImage::_initWithTiffData(void* pData, int nDatalen)
{
    QImage image;

    bool bRet = image.loadFromData((const uchar*)pData, nDatalen, "TIFF");
    if (!bRet)
        return false;

    // TODO: Better/faster conversion
    QImage convertedImg = image.convertToFormat(
                QImage::Format_ARGB32_Premultiplied);
    convertedImg = convertedImg.rgbSwapped();

    m_bPreMulti = true;
    m_bHasAlpha = convertedImg.hasAlphaChannel();
    m_nBitsPerComponent = cocos2d::getBitsPerComponent(convertedImg.format());
    m_nHeight = (short)convertedImg.height();
    m_nWidth = (short)convertedImg.width();
    m_pData  = new unsigned char[convertedImg.byteCount()];
    memcpy(m_pData, convertedImg.bits(), convertedImg.byteCount());

    return bRet;
}

bool CCImage::_saveImageToPNG(const char * pszFilePath, bool bIsToRGB)
{
    QImage image(m_pData, m_nWidth, m_nHeight, QImage::Format_ARGB32);

    if(bIsToRGB)
        image = image.convertToFormat(QImage::Format_RGB32);

    image = image.rgbSwapped();
    bool bRet = image.save(pszFilePath, "PNG");

    return bRet;
}

bool CCImage::_saveImageToJPG(const char * pszFilePath)
{
    QImage image(m_pData, m_nWidth, m_nHeight, QImage::Format_ARGB32);

    image = image.rgbSwapped();
    bool bRet = image.save(pszFilePath, "JPG");

    return bRet;
}

NS_CC_END;

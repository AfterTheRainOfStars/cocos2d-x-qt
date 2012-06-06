/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#include "CCSAXParser.h"
#ifdef Q_OS_SYMBIAN
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>
#include <QVector>
#else
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
//#include "CCLibxml2.h"
#endif // Q_OS_SYMBIAN
#include "CCFileUtils.h"

#include <memory.h>

NS_CC_BEGIN;

#ifdef Q_OS_SYMBIAN
class CCXmlContentHandler: public QXmlDefaultHandler
{
public:
    CCXmlContentHandler(CCSAXParser* pParser):
        m_pParser(pParser)
    {
    }

    virtual ~CCXmlContentHandler()
    {
    }

    virtual bool characters(const QString& ch)
    {
        QByteArray text = ch.toLatin1();
        CCSAXParser::textHandler(m_pParser, (const CC_XML_CHAR*)text.constData(), text.length());

        return true;
    }

    virtual bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName)
    {
        QByteArray name = qName.toLatin1();
        CCSAXParser::endElement(m_pParser, (const CC_XML_CHAR*)name.constData());

        return true;
    }

    virtual bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts)
    {
        int attributeCount = atts.count();
        int attributeCount2 = attributeCount * 2;

        QVector<QByteArray> bav(attributeCount2);
        QVector<const char*> strv(attributeCount2+1);
        if(attributeCount > 0)
        {
            for(int i = 0; i < attributeCount; i++)
            {
                bav[i*2+0] = atts.qName(i).toLatin1();
                bav[i*2+1] = atts.value(i).toLatin1();
            }

            for(int i = 0; i < attributeCount2; i++)
                strv[i] = bav[i].constData();
        }

        strv[attributeCount2] = NULL;

        QByteArray name = qName.toLatin1();
        CCSAXParser::startElement(m_pParser, (const CC_XML_CHAR*)name.constData(), (const CC_XML_CHAR**)&strv[0]);

        return true;
    }
private:
    CCSAXParser* m_pParser;
};
#endif

CCSAXParser::CCSAXParser()
{
    m_pDelegator = NULL;

}

CCSAXParser::~CCSAXParser(void)
{
}

bool CCSAXParser::init(const char *pszEncoding)
{
    CC_UNUSED_PARAM(pszEncoding);
    // nothing to do
    return true;
}

bool CCSAXParser::parse(const char* pXMLData, unsigned int uDataLength)
{
#ifdef Q_OS_SYMBIAN
    /*
    QFile xmlFile(pszFile);
    xmlFile.open(QIODevice::ReadOnly);
    if(!xmlFile.isOpen())
    {
        return false;
    }
    */

    QByteArray xmlBA(pXMLData, uDataLength);

    CCXmlContentHandler xmlContentHandler(this);
    //QXmlInputSource xmlSource(&xmlFile);
    QXmlInputSource xmlSource;
    //xmlSource.setData(pXMLData);
    xmlSource.setData(xmlBA);

    QXmlSimpleReader xmlReader;
    xmlReader.setContentHandler(&xmlContentHandler);
    xmlReader.parse(xmlSource);

    return true;
#else
    unsigned long size = uDataLength;
    const char *pBuffer = pXMLData;

    if (!pBuffer)
    {
        return false;
    }

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION
    xmlSAXHandler saxHandler;
    memset( &saxHandler, 0, sizeof(saxHandler) );
    // Using xmlSAXVersion( &saxHandler, 2 ) generate crash as it sets plenty of other pointers...
    saxHandler.initialized = XML_SAX2_MAGIC;  // so we do this to force parsing as SAX2.
    saxHandler.startElement = &CCSAXParser::startElement;
    saxHandler.endElement = &CCSAXParser::endElement;
    saxHandler.characters = &CCSAXParser::textHandler;

    int result = xmlSAXUserParseMemory( &saxHandler, this, pBuffer, size );
    if ( result != 0 )
    {
        return false;
    }
    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
    xmlMemoryDump();
#endif

    return true;
#endif
}

bool CCSAXParser::parse(const char *pszFile)
{
#ifdef Q_OS_SYMBIAN
    QFile xmlFile(pszFile);
    xmlFile.open(QIODevice::ReadOnly);
    if(!xmlFile.isOpen())
    {
        return false;
    }

    CCXmlContentHandler xmlContentHandler(this);
    QXmlInputSource xmlSource(&xmlFile);

    QXmlSimpleReader xmlReader;
    xmlReader.setContentHandler(&xmlContentHandler);
    xmlReader.parse(xmlSource);

    return true;
#else
    CCFileData data(pszFile, "rt");
    unsigned long size = data.getSize();
    char *pBuffer = (char*) data.getBuffer();

    if (!pBuffer)
    {
        return false;
    }

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION
    xmlSAXHandler saxHandler;
    memset( &saxHandler, 0, sizeof(saxHandler) );
    // Using xmlSAXVersion( &saxHandler, 2 ) generate crash as it sets plenty of other pointers...
    saxHandler.initialized = XML_SAX2_MAGIC;  // so we do this to force parsing as SAX2.
    saxHandler.startElement = &CCSAXParser::startElement;
    saxHandler.endElement = &CCSAXParser::endElement;
    saxHandler.characters = &CCSAXParser::textHandler;

    int result = xmlSAXUserParseMemory( &saxHandler, this, pBuffer, size );
    if ( result != 0 )
    {
        return false;
    }
    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
    xmlMemoryDump();
#endif

    return true;
#endif
}

void CCSAXParser::startElement(void *ctx, const CC_XML_CHAR *name, const CC_XML_CHAR **atts)
{
    ((CCSAXParser*)(ctx))->m_pDelegator->startElement(ctx, (char*)name, (const char**)atts);
}

void CCSAXParser::endElement(void *ctx, const CC_XML_CHAR *name)
{
    ((CCSAXParser*)(ctx))->m_pDelegator->endElement(ctx, (char*)name);
}
void CCSAXParser::textHandler(void *ctx, const CC_XML_CHAR *name, int len)
{
    ((CCSAXParser*)(ctx))->m_pDelegator->textHandler(ctx, (char*)name, len);
}
void CCSAXParser::setDelegator(CCSAXDelegator* pDelegator)
{
    m_pDelegator = pDelegator;
}

NS_CC_END;

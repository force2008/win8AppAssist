using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using json;

namespace JsonXmlConverter
{
    public sealed class Converter
    {
        public static String Json2Xml(String sJson)
        {
            return jsonxml.json2xml(sJson);
        }

        public static String Xml2Json(String sXml)
        {
            return jsonxml.xml2json(sXml);
        }
    }
}

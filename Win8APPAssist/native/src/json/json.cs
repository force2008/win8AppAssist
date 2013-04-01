using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using Newtonsoft.Json;

namespace json
{
    public sealed class jsonxml
    {
        public static string json2xml(string sJson)
        {
            try
            {
                return JsonConvert.DeserializeXNode(sJson).ToString();
            }
            catch (Exception ex)
            {
                return "";
            }
        }

        public static string xml2json(string sXml)
        {
            XmlReaderSettings setting = new XmlReaderSettings();
            setting.IgnoreComments = true;
            setting.IgnoreWhitespace = true;
            setting.DtdProcessing = DtdProcessing.Ignore;
            string result = "{}";
            try
            {
                StringReader strReader = new StringReader(sXml);
                XmlReader reader = XmlReader.Create(strReader, setting);
                XDocument doc = XDocument.Load(reader);
                result = Newtonsoft.Json.JsonConvert.SerializeXNode(doc);
            }
            catch (Exception e)
            {
                result = "{}";
            }
            return result;
        }
    }
}

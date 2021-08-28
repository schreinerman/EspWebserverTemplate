import os
import htmlmin

strInitScript = "#if defined(ARDUINO_ARCH_ESP8266)\r\n"
strInitScript += "  ESP8266WebServer* _pServer;\r\n"
strInitScript += "#else\r\n"
strInitScript += "  WebServer* _pServer;\r\n"
strInitScript += "#endif\r\n"
strInitScript += "#if defined(ARDUINO_ARCH_ESP8266)\r\n"
strInitScript += "  void HtmlFs_Init(ESP8266WebServer* pServer)\r\n"
strInitScript += "#else\r\n"
strInitScript += "  void HtmlFs_Init(WebServer* pServer)\r\n"
strInitScript += "#endif\r\n"
strInitScript += "{\r\n"
strInitScript += "    _pServer = pServer;\r\n"
strInitScript += "    _pServer->on(\"/\", []() {\r\n"
strInitScript += "        Esp32Wifi_KeepAlive();\r\n"
strInitScript += "        _pServer->send(200, \"text/html\", (char*)au8index_html);\r\n"
strInitScript += "    });\r\n"
htmlFsFile = open('htmlfs.cpp', 'w') 
htmlFsFile.write("#include \"htmlfs.h\"\r\n")
htmlFsFile.write("#include \"esp32wifi.h\"\r\n")
htmlFsFile.write("#include <stdint.h>\r\n")
htmlFsFile.write("#include <Arduino.h>\r\n")
htmlFsFile.write("#if defined(ARDUINO_ARCH_ESP8266)\r\n")
htmlFsFile.write("  #include <ESP8266WebServer.h>\r\n")
htmlFsFile.write("#else\r\n")
htmlFsFile.write("  #include <WebServer.h>\r\n")
htmlFsFile.write("#endif\r\n")

for subdir, dirs, files in os.walk("html"):
    for filename in files:
        filepath = subdir + os.sep + filename
        if (filename != ".DS_Store"):
            fileName = filepath

            fileType = "text/plain"
            if fileName.endswith("html"):
                fileType = "text/html"
            if fileName.endswith("xml"):
                fileType = "text/xml"
            if fileName.endswith("png"):
                fileType = "image/png"
            if fileName.endswith("jpg"):
                fileType = "image/jpeg"
            if fileName.endswith("svg"):
                fileType = "image/svg"
            if fileName.endswith("ico"):
                fileType = "image/ico"

            strFileName = fileName[5:]
            strInitScript += "    _pServer->on(\"/" + strFileName + "\", []() {\r\n"
            strInitScript += "        Esp32Wifi_KeepAlive();\r\n"
            strFileName = strFileName.replace("/","_")
            strFileName = strFileName.replace(".","_")
            strFileName = strFileName.replace("-","_")
            if (fileType.startswith("text")):
                strInitScript += "        _pServer->send(200, \"" + fileType + "\", (char*)au8" + strFileName + ");\r\n"
            else:
                fileSize = os.path.getsize(fileName)
                strInitScript += "        _pServer->send_P(200, \"" + fileType + "\", (char*)au8" + strFileName + "," + str(fileSize) + ");\r\n"
            strInitScript += "    });\r\n"

            if (fileType.startswith("text")):
                readFile = open(fileName, 'r') 
                Lines = readFile.readlines() 
                readFile.close()
                htmlFsFile.write("const PLACE_IROM char au8" + strFileName + "[] = ")


                count = 0
                # Strips the newline character 
                for line in Lines:
                    lineTmp = line
                    if (fileType.endswith("html")):
                        try:
                            lineTmp = htmlmin.minify(lineTmp,remove_comments=True, remove_empty_space=True,remove_all_empty_space=True)
                        except:
                            print("Could not optimize html")
                    lineTmp = lineTmp.replace("\"","\\\"")
                    lineTmp = lineTmp.replace("\r\n","")
                    lineTmp = lineTmp.replace("\n","")
                    lineTmp = "\"" + lineTmp + "\\r\\n\"\r\n"
                    htmlFsFile.write(lineTmp)

                htmlFsFile.write(";\r\n")
            else:
                htmlFsFile.write("const PLACE_IROM uint8_t au8" + strFileName + "[" + str(fileSize) + "] = {")
                readBinFile = open(fileName, 'rb');
                byte = readBinFile.read(1)
                while byte:
                    htmlFsFile.write(hex(bytearray(byte)[0]) + ", ")
                    byte = readBinFile.read(1)
                htmlFsFile.write("};\r\n")    

strInitScript += "}\r\n"

htmlFsFile.write("\r\n" + strInitScript);


htmlFsFile.close()

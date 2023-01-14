raw_literal_delimiter = "rawliteral"
header = '#include "Arduino.h"\n\n\nconst char index_html[] PROGMEM = R"' + raw_literal_delimiter + '(\n'
footer = '\n)' + raw_literal_delimiter + '";'
html_file = "resources\LED_HUE_index.html"
destination_file = "src\web\generated\index.h"

f = open(html_file, "r")
html_content = f.read()
f.close()

f = open(destination_file, "w")
f.write(header)
f.write(html_content)
f.write(footer)
f.close()
files = [
    ("resources\index.html", "src\web\generated\index_html.h"),
    ("resources\debug.html", "src\web\generated\debug_html.h"),
]


def transpile_file(source_file, destination_file):
    raw_literal_delimiter = "rawliteral"
    variable_name = source_file.split("\\")[-1].replace(".", "_")
    header = '#include "Arduino.h"\n\n\nconst char ' + variable_name + '[] PROGMEM = R"' + raw_literal_delimiter + '(\n'
    footer = '\n)' + raw_literal_delimiter + '";'

    f = open(source_file, "r")
    html_content = f.read()
    f.close()

    f = open(destination_file, "w")
    f.write(header)
    f.write(html_content)
    f.write(footer)
    f.close()
    print('Transpiled "' + source_file + '" to "' + destination_file + '"')


def main():
    print("Running HTML transpiling script...")
    for file_tuple in files:
        transpile_file(file_tuple[0], file_tuple[1])


if __name__ == "__main__":
    main()

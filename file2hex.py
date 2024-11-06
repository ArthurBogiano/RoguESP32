import gzip
import binascii
import sys
from pathlib import Path


def convert_to_header(gz_file):
    """Converte o arquivo gzip para um array hexadecimal em um arquivo de cabeçalho .h."""
    array_name = gz_file.stem.replace('.', '_')  # Nome do array baseado no nome do arquivo
    with open(gz_file, 'rb') as f:
        byte_content = f.read()

    hex_array = ", ".join(f"0x{byte:02x}" for byte in byte_content)
    array_length = len(byte_content)

    header_content = f"""
const char {array_name}[] PROGMEM = {{
    {hex_array}
}};
"""
    print(header_content)


def main(input_file_path):
    input_file = Path(input_file_path)
    if not input_file.exists():
        print("Arquivo de entrada não encontrado!")
        sys.exit(1)

    # Gera o arquivo .h com o array hexadecimal
    convert_to_header(input_file)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python script.py caminho_do_arquivo.html")
        sys.exit(1)

    input_file_path = sys.argv[1]
    main(input_file_path)

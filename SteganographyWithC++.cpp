#include <iostream>
#include <fstream>

using namespace std;

void hideMessageInImage(const string& imagePath, const string& message) {
    ifstream imageFile(imagePath, ios::binary);
    if (!imageFile) {
        cerr << "Erro ao abrir a imagem" << endl;
        return;
    }

    unsigned char header[54];
    imageFile.read(reinterpret_cast<char*>(header), 54);

    ofstream outputFile("output.png", ios::binary);
    if (!outputFile) {
        cerr << "Erro ao criar o arquivo de saída" << endl;
        return;
    }

    outputFile.write(reinterpret_cast<const char*>(header), 54);

    int byteIndex = 0;
    char byte = message[byteIndex++];
    int bitIndex = 0;

    unsigned char pixel[3];
    while (imageFile.read(reinterpret_cast<char*>(pixel), 3)) {
        for (int i = 0; i < 3; ++i) {
            pixel[i] = (pixel[i] & 0xFE) | ((byte >> bitIndex) & 0x01);
            ++bitIndex;
            if (bitIndex >= 8) {
                byte = message[byteIndex++];
                bitIndex = 0;
            }
        }

        outputFile.write(reinterpret_cast<const char*>(pixel), 3);
    }
    imageFile.close();
    outputFile.close();

    cout << "Mensagem ocultada na imagem com sucesso!" << endl;
}

int main() {
    cout << "Esteganografia - Ocultar mensagem em uma imagem" << endl;

    string imagePath;
    cout << "Digite o caminho da imagem de origem: ";
    cin >> imagePath;

    string message;
    cout << "Digite a mensagem a ser ocultada na imagem: ";
    cin.ignore();
    getline(cin, message);

    hideMessageInImage(imagePath, message);

    return 0;
}

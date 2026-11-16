#include <windows.h>
#include <stdio.h>
#include <string.h>
#define NOME 1
#define ESTADO 2
#define MASC 3
#define FEM 4
#define SALVAR 5
#define LISTAR 6
#define LIMPAR 7
HWND txtNome, comboEstado, radioM, radioF;
LRESULT CALLBACK Janela(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
 switch (msg) {
 case WM_CREATE:
 CreateWindow("STATIC", "Nome:", WS_VISIBLE | WS_CHILD, 20, 20, 100, 25, hwnd, NULL, NULL,
NULL);
 txtNome = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 20, 220, 25,
hwnd, (HMENU)NOME, NULL, NULL);
 CreateWindow("STATIC", "Estado Civil:", WS_VISIBLE | WS_CHILD, 20, 60, 100, 25, hwnd,
NULL, NULL, NULL);
 comboEstado = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 120,
60, 220, 120, hwnd, (HMENU)ESTADO, NULL, NULL);
 SendMessage(comboEstado, CB_ADDSTRING, 0, (LPARAM)"Solteiro");
 SendMessage(comboEstado, CB_ADDSTRING, 0, (LPARAM)"Casado");
 SendMessage(comboEstado, CB_ADDSTRING, 0, (LPARAM)"Divorciado");
 SendMessage(comboEstado, CB_ADDSTRING, 0, (LPARAM)"Viuvo");
 SendMessage(comboEstado, CB_SETCURSEL, 0, 0);
 CreateWindow("STATIC", "Sexo:", WS_VISIBLE | WS_CHILD, 20, 105, 100, 25, hwnd, NULL, NULL,
NULL);
 radioM = CreateWindow("BUTTON", "Masculino", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON |
WS_GROUP, 120, 105, 100, 25, hwnd, (HMENU)MASC, NULL, NULL);
 radioF = CreateWindow("BUTTON", "Feminino", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
230, 105, 100, 25, hwnd, (HMENU)FEM, NULL, NULL);
 SendMessage(radioM, BM_SETCHECK, BST_CHECKED, 0);
 CreateWindow("BUTTON", "Salvar", WS_VISIBLE | WS_CHILD, 20, 160, 100, 30, hwnd,
(HMENU)SALVAR, NULL, NULL);
 CreateWindow("BUTTON", "Listar", WS_VISIBLE | WS_CHILD, 135, 160, 100, 30, hwnd,
(HMENU)LISTAR, NULL, NULL);
 CreateWindow("BUTTON", "Limpar", WS_VISIBLE | WS_CHILD, 250, 160, 100, 30, hwnd,
(HMENU)LIMPAR, NULL, NULL);
 break;
 case WM_COMMAND:
 if (LOWORD(wp) == SALVAR) {
 char nome[100], estado[50], sexo[20];
 GetWindowText(txtNome, nome, sizeof(nome));
 int pos = (int)SendMessage(comboEstado, CB_GETCURSEL, 0, 0);
 SendMessage(comboEstado, CB_GETLBTEXT, pos, (LPARAM)estado);
 strcpy(sexo, SendMessage(radioM, BM_GETCHECK, 0, 0) == BST_CHECKED ? "Masculino" :
"Feminino");
 if (nome[0] == '\0') {
 MessageBox(hwnd, "Digite o nome!", "Aviso", MB_OK);
 break;
 }
 FILE *arquivo = fopen("cadastro.txt", "a");
 if (arquivo == NULL) {
 MessageBox(hwnd, "Erro ao abrir arquivo!", "Erro", MB_OK);
 break;
 }
 fprintf(arquivo, "%s;%s;%s\n", nome, estado, sexo);
 fclose(arquivo);
 MessageBox(hwnd, "Cadastro salvo!", "Sucesso", MB_OK);
 }
 if (LOWORD(wp) == LISTAR) {
 FILE *arquivo = fopen("cadastro.txt", "r");
 if (arquivo == NULL) {
 MessageBox(hwnd, "Nenhum cadastro encontrado!", "Aviso", MB_OK);
 break;
 }
 char linha[300], resultado[10000] = "";
 while (fgets(linha, sizeof(linha), arquivo)) {
 if (strlen(resultado) + strlen(linha) < sizeof(resultado) - 1)
 strcat(resultado, linha);
 }
 fclose(arquivo);
 MessageBox(hwnd, resultado[0] ? resultado : "Arquivo vazio.", "Cadastros", MB_OK);
 }
 if (LOWORD(wp) == LIMPAR) {
 SetWindowText(txtNome, "");
 SendMessage(comboEstado, CB_SETCURSEL, 0, 0);
 SendMessage(radioM, BM_SETCHECK, BST_CHECKED, 0);
 SendMessage(radioF, BM_SETCHECK, BST_UNCHECKED, 0);
 }
 break;
 case WM_DESTROY:
 PostQuitMessage(0);
 break;
 default:
 return DefWindowProc(hwnd, msg, wp, lp);
 }
 return 0;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int show) {
 WNDCLASS wc = {0};
 wc.lpfnWndProc = Janela;
 wc.hInstance = hInst;
 wc.lpszClassName = "Cadastro";
 wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
 RegisterClass(&wc);
 HWND hwnd = CreateWindow("Cadastro", "Formulario de Cadastro", WS_OVERLAPPEDWINDOW, 300, 200,
400, 260, NULL, NULL, hInst, NULL);
 ShowWindow(hwnd, show);
 MSG msg;
 while (GetMessage(&msg, NULL, 0, 0)) {
 TranslateMessage(&msg);
 DispatchMessage(&msg);
 }
 return 0;
}

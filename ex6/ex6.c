#include <windows.h>
#include <tchar.h>
#include <windowsx.h>


/* ===================================================== */
/* Programa base (esqueleto) para aplicações Windows     */
/* ===================================================== */
// Cria uma janela de nome "Janela Principal" e pinta fundo de branco
// Modelo para programas Windows:
//  Composto por 2 funções: 
//	WinMain()     = Ponto de entrada dos programas windows
//			1) Define, cria e mostra a janela
//			2) Loop de recepção de mensagens provenientes do Windows
//     TrataEventos()= Processamentos da janela (pode ter outro nome)
//			1) É chamada pelo Windows (callback) 
//			2) Executa código em função da mensagem recebida

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);

// Nome da classe da janela (para programas de uma só janela, normalmente este nome é 
// igual ao do próprio programa) "szprogName" é usado mais abaixo na definição das 
// propriedades do objecto janela
TCHAR szProgName[] = TEXT("Base");

// ============================================================================
// FUNÇÃO DE INÍCIO DO PROGRAMA: _tWinMain()
// ============================================================================
// Em Windows, o programa começa sempre a sua execução na função WinMain()que desempenha
// o papel da função main() do C em modo consola WINAPI indica o "tipo da função" (WINAPI
// para todas as declaradas nos headers do Windows e CALLBACK para as funções de
// processamento da janela)
// Parâmetros:
//   hInst: Gerado pelo Windows, é o handle (número) da instância deste programa 
//   hPrevInst: Gerado pelo Windows, é sempre NULL para o NT (era usado no Windows 3.1)
//   lpCmdLine: Gerado pelo Windows, é um ponteiro para uma string terminada por 0
//              destinada a conter parâmetros para o programa 
//   nCmdShow:  Parâmetro que especifica o modo de exibição da janela (usado em  
//        	   ShowWindow()

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;		// hWnd é o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;		// MSG é uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para 
	// definir as características da classe da janela

// ============================================================================
// 1. Definição das características da janela "wcApp" 
//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
// ============================================================================
	wcApp.cbSize = sizeof(WNDCLASSEX);      // Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;		         // Instância da janela actualmente exibida 
	// ("hInst" é parâmetro de WinMain e vem 
		  // inicializada daí)
	wcApp.lpszClassName = szProgName;       // Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;       // Endereço da função de processamento da janela
	// ("TrataEventos" foi declarada no início e
	// encontra-se mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW;  // Estilo da janela: Fazer o redraw se for
	// modificada horizontal ou verticalmente

	wcApp.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);   // "hIcon" = handler do ícon normal		//alterar
	// "NULL" = Icon definido no Windows
	// "IDI_AP..." Ícone "aplicação"
	wcApp.hIconSm = LoadIcon(NULL, IDI_SHIELD); // "hIconSm" = handler do ícon pequeno		//alterar
	// "NULL" = Icon definido no Windows
	// "IDI_INF..." Ícon de informação
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato)		
	// "NULL" = Forma definida no Windows
	// "IDC_ARROW" Aspecto "seta" 
	wcApp.lpszMenuName = NULL;			// Classe do menu que a janela pode ter
	// (NULL = não tem menu)
	wcApp.cbClsExtra = 0;				// Livre, para uso particular
	wcApp.cbWndExtra = 0;				// Livre, para uso particular
	//wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);									//alterar
	wcApp.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 255, 150));
	// "hbrBackground" = handler para "brush" de pintura do fundo da janela. Devolvido por
	// "GetStockObject".Neste caso o fundo será branco

	// ============================================================================
	// 2. Registar a classe "wcApp" no Windows
	// ============================================================================
	if (!RegisterClassEx(&wcApp))
		return(0);

	// ============================================================================
	// 3. Criar a janela
	// ============================================================================
	hWnd = CreateWindow(
		szProgName,			// Nome da janela (programa) definido acima
		TEXT("Exemplo de Nova Janela Principal em C"),// Texto que figura na barra do título
		WS_OVERLAPPEDWINDOW,	// Estilo da janela (WS_OVERLAPPED= normal)
		CW_USEDEFAULT,		// Posição x pixels (default=à direita da última)
		CW_USEDEFAULT,		// Posição y pixels (default=abaixo da última)
		CW_USEDEFAULT,		// Largura da janela (em pixels)
		CW_USEDEFAULT,		// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir de
		// outra) ou HWND_DESKTOP se a janela for a primeira, 
		// criada a partir do "desktop"
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,		// handle da instância do programa actual ("hInst" é 
		// passado num dos parâmetros de WinMain()
		0);				// Não há parâmetros adicionais para a janela
	// ============================================================================
	// 4. Mostrar a janela
	// ============================================================================
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido por 
	// "CreateWindow"; "nCmdShow"= modo de exibição (p.e. 
	// normal/modal); é passado como parâmetro de WinMain()
	UpdateWindow(hWnd);		// Refrescar a janela (Windows envia à janela uma 
	// mensagem para pintar, mostrar dados, (refrescar)
// ============================================================================
// 5. Loop de Mensagens
// ============================================================================
// O Windows envia mensagens às janelas (programas). Estas mensagens ficam numa fila de
// espera até que GetMessage(...) possa ler "a mensagem seguinte"	
// Parâmetros de "getMessage":
// 1)"&lpMsg"=Endereço de uma estrutura do tipo MSG ("MSG lpMsg" ja foi declarada no  
//   início de WinMain()):
//			HWND hwnd		handler da janela a que se destina a mensagem
//			UINT message		Identificador da mensagem
//			WPARAM wParam		Parâmetro, p.e. código da tecla premida
//			LPARAM lParam		Parâmetro, p.e. se ALT também estava premida
//			DWORD time		Hora a que a mensagem foi enviada pelo Windows
//			POINT pt		Localização do mouse (x, y) 
// 2)handle da window para a qual se pretendem receber mensagens (=NULL se se pretendem
//   receber as mensagens para todas as
// janelas pertencentes à thread actual)
// 3)Código limite inferior das mensagens que se pretendem receber
// 4)Código limite superior das mensagens que se pretendem receber

// NOTA: GetMessage() devolve 0 quando for recebida a mensagem de fecho da janela,
// 	  terminando então o loop de recepção de mensagens, e o programa 

	while (GetMessage(&lpMsg, NULL, 0, 0) > 0) {
		TranslateMessage(&lpMsg);	// Pré-processamento da mensagem (p.e. obter código 
		// ASCII da tecla premida)
		DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta ao Windows, que
		// aguarda até que a possa reenviar à função de 
		// tratamento da janela, CALLBACK TrataEventos (abaixo)
	}

	// ============================================================================
	// 6. Fim do programa
	// ============================================================================
	return (int)lpMsg.wParam;	// Retorna sempre o parâmetro wParam da estrutura lpMsg
}

// ============================================================================
// FUNÇÃO DE PROCESSAMENTO DA JANELA
// Esta função pode ter um nome qualquer: Apenas é necesário que na inicialização da
// estrutura "wcApp", feita no início de // WinMain(), se identifique essa função. Neste
// caso "wcApp.lpfnWndProc = WndProc"
//
// WndProc recebe as mensagens enviadas pelo Windows (depois de lidas e pré-processadas
// no loop "while" da função WinMain()
// Parâmetros:
//		hWnd	O handler da janela, obtido no CreateWindow()
//		messg	Ponteiro para a estrutura mensagem (ver estrutura em 5. Loop...
//		wParam	O parâmetro wParam da estrutura messg (a mensagem)
//		lParam	O parâmetro lParam desta mesma estrutura
//
// NOTA:Estes parâmetros estão aqui acessíveis o que simplifica o acesso aos seus valores
//
// A função EndProc é sempre do tipo "switch..." com "cases" que descriminam a mensagem
// recebida e a tratar.
// Estas mensagens são identificadas por constantes (p.e. 
// WM_DESTROY, WM_CHAR, WM_KEYDOWN, WM_PAINT...) definidas em windows.h
// ============================================================================

typedef struct {
	HANDLE hMutex, hWnd;
	RECT dim;	// Limites da janela
	int stepX, stepY;	// Velocidade
	POINT pos; // x,y
	TCHAR letra;
	BOOL continua;
}DATA;

DWORD WINAPI moveLetra(LPVOID data) {
	DATA* pData = (DATA*)data;

	while (pData->continua) {
		WaitForSingleObject(pData->hMutex, INFINITE);

		if (pData->stepX != 0) {
			pData->pos.x += pData->stepX;
			if (pData->pos.x < 0) {        //LIMITE ESQUERDO
				pData->pos.x = 0;
				pData->stepX += -1;
			}
			else if (pData->pos.x > pData->dim.right) {    //LIMITE DIREITO
				pData->pos.x = pData->dim.right;
				pData->stepX += -1;
			}
		}

		InvalidateRect(pData->hWnd, NULL, TRUE);

		ReleaseMutex(pData->hMutex);
		Sleep(33);
	}

	return 0;
}

LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {

	int res;
	//static TCHAR letra = _T('?');
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR letra;
	static DATA data;
	HANDLE hThread;

	switch (messg) {
	case WM_CREATE:
		data.hMutex = CreateMutex(NULL, FALSE, NULL);
		data.hWnd = hWnd;
		GetClientRect(hWnd, &data.dim);
		data.stepX = 2;
		data.stepY = 0;
		data.pos.x = data.dim.right / 2;
		data.pos.y = data.dim.bottom / 2;
		data.letra = _T("O");
		data.continua = TRUE;

		hThread = CreateThread(NULL, 0, moveLetra, (LPVOID)&data, 0, NULL);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextColor(hdc, RGB(0, 0, 0));	//PRETO
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, data.pos.x, data.pos.y, &data.letra, 1);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_BACK) {
			data.pos.x = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CLOSE:
		res = MessageBox(hWnd, _T("Pretende mesmo sair?"), _T("Confirmação"), MB_ICONQUESTION | MB_YESNO | MB_TASKMODAL);
		if (res == IDYES) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_CHAR:
		letra = LOWORD(wParam);
		switch (letra)
		{
		case VK_SPACE:
			//inverter novamente
			//InvalidateRect(hWnd, );
			break;
		case '+':
			//aumenta velocidade
			break;
		case '-':
			//diminui velocidade
			break;
		}
		break;
	case WM_SIZE:
		WaitForSingleObject(data.hMutex, INFINITE);
		GetClientRect(hWnd, &data.dim);
		data.pos.x = data.dim.right / 2;
		data.pos.y = data.dim.bottom / 2;
		ReleaseMutex(data.hMutex);
		break;
	case WM_LBUTTONDOWN:
		data.pos.x = GET_X_LPARAM(lParam);			//LWORD(lparam)
		data.pos.y = GET_Y_LPARAM(lParam);			//HWORD(lparam)
		InvalidateRect(hWnd, NULL, FALSE);
		//hdc = GetDC(hWnd);
		//SetTextColor(hdc, RGB(0, 0, 0));	//PRETO
		//SetBkMode(hdc, TRANSPARENT);
		//TextOut(hdc, x, y, &letra, 1);
		//ReleaseDC(hWnd, hdc);
		break;

	case WM_DESTROY: // Destruir a janela e terminar o programa 
		// "PostQuitMessage(Exit Status)"
		//WaitForSingleObject(hThread, INFINITE);
		CloseHandle(data.hMutex);
		data.continua = 0;
		PostQuitMessage(0);
		break;
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar","maximizar","restaurar")
		// não é efectuado nenhum processamento, apenas se segue o "default" do Windows
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;  // break tecnicamente desnecessário por causa do return
	}
	return 0;
}

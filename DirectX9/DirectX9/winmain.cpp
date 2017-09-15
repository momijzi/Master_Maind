
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>

#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include"time.h"

#define screenWidth 640
#define screenHeight 800
#define Pixel 48

//ウィンドウプロシージャ
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//ウィンドウのハンドル
	UINT msg,	//メッセージ
	WPARAM wParam,
	LPARAM lParam
)
{
	//メッセージ処理のための関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に
	//各ウィンドウに設定される

	//アプリケーションがメッセージを
	//取得したら呼び出される

	//メッセージの例

	//WM_DESTROY ウィンドウが破棄された
	//			 ×ボタンが押された時など
	//WM_MOVE    ウィンドウが移動
	//WM_SIZE    ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた
	//処理を行う

	//取りあえず WM_DESTROYが来た時の
	//終了処理のみ定義

	switch (msg)
	{
		case WM_DESTROY:

			PostQuitMessage(0);

			//メッセージキューに
			//新しくWM_QUITメッセージを送る

			//いずれメッセージキューから
			//取り出され
			//メインループが終了する
			//(メインループの終了条件を
			//そのように作る)
			break;
	}

	//独自の処理を行ったら
	//デフォルトのウィンドウプロシージャを実行

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウ設定の名前
static const TCHAR* WC_BASIC = _T("Basic");

//作りたいウィンドウの設定を
//登録する関数
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;
	
	//wcexの先頭アドレスから
	//WNDCLASSEXの長さ分のメモリを
	//全て0で埋める
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//つまりはwcexが使用しているメモリ領域を
	//全て0で初期化する処理

	//ウィンドウの設定

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウの設定情報に追加する捕捉を付けないので
	//0で固定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Windowのスタイル
	//それぞれがビット単位のフラグであるため
	//オア演算で合成したものがスタイルの最終的な設定
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW 横サイズが変わったときウィンドウ全体の再描画
	//   VREDRAW 縦サイズが変わったときウィンドウ全体の再描画
	//   DBLCLKS ダブルクリックが発生したことをウィンドウに通知

	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;

	//ウィンドウプロシージャのアドレス
	//関数ポインタの設定
	//ウィンドウプロシージャをOSが呼び出せるようにする
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//背景色
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//メニュー　なし
	wcex.lpszMenuName = NULL;

	//ウィンドウクラスの名前　このウィンドウ設定の名前
	wcex.lpszClassName = WC_BASIC;

	//アイコン　デフォルトのを使う
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0, 
		0, 
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//カーソル デフォルトのものを使う
	wcex.hCursor= (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//設定したウィンドウクラスを登録
	//登録したウィンドウ設定でないと
	//ウィンドウを作ることができない
	return RegisterClassEx(&wcex); 
	//HRESULT型の実行結果が戻される
}


//登録した設定を基にウィンドウを作成する
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMainの識別子
	HWND &refHWnd,	//ウィンドウの識別子
				   //正しくウィンドウの作成ができたら
				   //この変数に識別子を代入する
	int width = screenWidth,	//クライアント領域の幅
	int height = screenHeight)	//クライアント領域の高さ
	//クライアント領域はウィンドウ全体から
	//外枠やメニューの部分を除いた物と今は思っておけばOK
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//クライアント領域を保持するのに必要な
	//ウィンドウのおおきさを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//正しく実行されるとrectの値が更新される

	//WS_OVERLAPPEDWINDOWはウィンドウのスタイルの一つ

	//ウィンドウのハンドル
	//ウィンドウの識別に使用する
	HWND hWnd;

	hWnd = CreateWindowEx
	(	0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//作りたいウィンドウクラス
								//あらかじめ登録されたもの
		_T("タイトル"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
		CW_USEDEFAULT,			//位置x座標 デフォルトの値
		CW_USEDEFAULT,			//位置y座標 デフォルトの値
		rect.right - rect.left,	//ウィンドウ全体の幅
		rect.bottom - rect.top, //ウィンドウ全体の高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューのハンドル
		hInstance,				//WinMainのハンドル

		NULL);					//ウィンドウ作成データ
								//ウィンドウに好きな32ビットのデータを
								//付属させることができる
								//追加情報構造体へのポインタなど
								//を付けるのが無難な使い方か
								//今回は使わない

	//ウィンドウが正しく作れたかをチェック
	//正しく作れたらhWndにNULL以外が入るはず
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT型　失敗を表す識別子の一つ
	}

	//ウィンドウをアクティブにして表示
	//hWndでウィンドウの識別をしていることに注目
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//クライアント領域の更新
	UpdateWindow(hWnd);

	//参照渡しになっている引数に
	//ウィンドウの識別ハンドルをコピーする
	//これによって今作ったウィンドウの識別ハンドルを
	//呼び出し元のプログラムで得ることができる
	refHWnd = hWnd;

	//HRESULT型　成功を表す識別子の一つ
	return S_OK;
}

//スプライトのインスタンスを作成
//パラメータは適当で
Sprite sprite;
Sprite sprite2;
Texture textureColor;
Texture textureResult;
Texture textureStart;
Texture textureOver;
Texture textureClear;



//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
   (HINSTANCE hInstance,	//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine,		//コマンドライン引数
	   int nCmdShow)		//ウィンドウの表示状態
{
	//変数の宣言----------------------------------------
	
	int splitTextureX = 0;	//表示したい画像の位置（分割されているため）
	int HeightTexture = 1;	//答えた後次の行描写
	int trueCount = 0;		//正解している数
	int falseCount = 0;		//不正解だが色はある
	int Count = 0;			//正当率
	int PlayerX = 0;		//プレイヤーが操作したい位置の座標のための変数
	int textureCr[44];		//各色を格納
	int ResultTexture[44];	//正当率描画用
	bool flag = false;		//場所はあっていないが色はあっている

	srand((unsigned int)time(NULL));

	enum GameMode{ZERO,START,PLAY,OVER};
	GameMode game = ZERO;

	
	
	////メッセージボックス
	//MessageBox(NULL,		//ウィンドウのハンドル 
	//	TEXT("最初の設定に成功しましたぱちぱち"),		//本文
	//	TEXT( "テスト-タイトル"),//タイトル
	//	MB_OK);				//メッセージボックスのタイプ
	//						//MB_OK  okのボタンが表示

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,	
			TEXT("ウィンドウクラス設定失敗"),		
			TEXT("テスト-タイトル"),
			MB_OK);				
		return 0;
	}

	HWND hWnd = NULL;

	/*正しくウィンドウが作成されれば
	hWndにウィンドウの識別ハンドルが入る*/
	if (FAILED(MakeWindow(hInstance,hWnd)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウ作成失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}
	
	//Direct3Dを管理するクラス(シングルトン)への参照を取得
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDeviceの作成を試みる
	if (d3d.TryCreate(hWnd))
	{
		MessageBox(NULL,
			TEXT("Direct3D Device作成成功"),
			TEXT("テスト-タイトル"),
			MB_OK);
	}

	//レンダーステートの設定  αブレンド
	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);

	
	sprite.SetAlpha(0.1);						//透明度の設定
	sprite.SetSize(Pixel, Pixel);				//画像の大きさ
	sprite.SetAngle(0);							//画像の回転

	sprite2.SetAlpha(0.1);						//透明度の設定
	sprite2.SetSize(screenWidth/2, screenHeight/4);				//画像の大きさ
	sprite2.SetAngle(0);						//画像の回転


												//テクスチャのインスタンスを作成
	textureColor.Load(_T("Texture/color.png"));			//丸の色と□とA
	textureResult.Load(_T("Texture/resultMark.png"));	//正解数を表示
	textureStart.Load(_T("Texture/start.png"));			//スタート画像
	textureOver.Load(_T("Texture/over.bmp"));			//失敗画像
	textureClear.Load(_T("Texture/clear.png"));			//クリア画像



	//ここで読み込んだ画像の映す場所を指定できる
	textureColor.SetDivide(8, 0);//8分割する
	textureResult.SetDivide(2, 0);//2分割する
		

	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	//メインループ
	//メッセージループ

	MSG msg = {};

	//quitメッセージが出てくるまでループを繰り返す
	//quitメッセージは上記のウィンドウプロシージャから送信
	//送信の条件などはウィンドウプロシージャを確認
	while (msg.message != WM_QUIT)
	{
		
		//PeekMessage
		//メッセージキューの中にメッセージがあるかを調べて
		//msgの中に書き込みtrueを返す
		//メッセージがなければfalseを返す
		//PM_REMOVEを設定しているとメッセージの書き込みのあとに
		//メッセージキューから元のメッセージを消す

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換し
			//それをメッセージキューにポストする
			TranslateMessage(&msg);

			//メッセージの割り当て
			//ウィンドウプロシージャでメッセージを処理
			DispatchMessage(&msg);
		}
		else
		{
			//メッセージキューにメッセージが無かったとき
			//こちらの処理
			//ここにゲーム処理を書き込んでいく
			
			pDi->Update();//キー状態の更新

			if (pDi->KeyJustPressed(DIK_A))
			{
				MessageBox(NULL,
					TEXT("キー入力確認"),
					TEXT("テスト-タイトル"),
					MB_OK);
			}

			if (pDi->MouseButtonJustPressed(MOUSE_BUTTON_LEFT))
			{
				Vector2<int> vec = pDi->MousePosition();
				int breakpoint = 0;
			}

			//sprite.SetAngle(sprite.GetAngle_Rad()+0.0f);//回転ここのfを大きい値にするほど回転速度が上がる

			static int dir = 1;//α
			sprite.SetAlpha(sprite.GetAlpha() + (dir*0.01));
			switch (dir)
			{
				case 1:
					if (sprite.GetAlpha() >= 1)
					{
						dir = -1;
					}
					break;
				case -1:
					if (sprite.GetAlpha() <= 0)
					{
						dir = 1;
					}
					break;
			}

			int TexturePos = (4 * HeightTexture);	//現在描画しているところ(4 * HeightTexture)
			int TexturePos2 = (4 - PlayerX);		//現在操作している場所　横軸 (4 - PlayerX)
	

			switch (game)
			{
				case ZERO://乱数など初期に設定する必要のある物を取得
				 //ここで答え作成

					d3d.ClearScreen();

				 //配列初期化
					for (int i = 0; i < 44; i++)
					{
						textureCr[i] = -1;
						ResultTexture[i] = 2;
					}
					Count = 0;
					HeightTexture = 1;
					game = START;
					break;
				case START:

					if (pDi->KeyJustPressed(DIK_RETURN))
					{
						//答えを指定　乱数
						textureCr[40] = rand() % 6;
						textureCr[41] = rand() % 6;
						textureCr[42] = rand() % 6;
						textureCr[43] = rand() % 6;

						game = PLAY;
					}
					break;
				case PLAY:
					//左入力したとき横に移動
					if(pDi->KeyJustPressed(DIK_LEFT))
					{
						//0以外でないと動かない
						if (PlayerX != 0)
						{
							PlayerX--;
						}
					}
					else if (pDi->KeyJustPressed(DIK_RIGHT))
					{
						//４以外でないと動かない
						if (PlayerX != 4)
						{
							PlayerX++;
						}
					}
					//上入力をしたとき色の変更
					else if (pDi->KeyJustPressed(DIK_UP) && PlayerX != 4)
					{
						
						//０の時５にしてループ
						if (textureCr[TexturePos - TexturePos2] /*現在指定している所*/ <= 0)
						{
							textureCr[TexturePos - TexturePos2] = 5;
						}
						//０以外ならマイナスループ
						else
						{
							textureCr[TexturePos - TexturePos2]--;
						}
					}
					//下入力したとき色を変更
					else if(pDi->KeyJustPressed(DIK_DOWN) && PlayerX != 4)
					{
						//５の時０にしてループ
						if (textureCr[TexturePos- TexturePos2] == 5)
						{
							textureCr[TexturePos - TexturePos2] = 0;
						}
						//５以外の時はプラスループ
						else
						{
							textureCr[TexturePos - TexturePos2]++;
						}
					}
					
					
					break;
				case OVER:
					if (pDi->KeyJustPressed(DIK_RETURN))
					{
						game = ZERO;
					}

					
					break;
			}
			//描画処理
			if (SUCCEEDED(d3d.BeginScene()))
			{
				//バックバッファのクリア

				d3d.ClearScreen();
				if (game == PLAY )
				{
					//判定を書いていく
					//まずは正解ドンピシャの場合//エンターを押したとき判定
					if (pDi->KeyJustPressed(DIK_RETURN) && PlayerX == 4 && textureCr[TexturePos - 4] != -1
						&& textureCr[TexturePos - 3] != -1 && textureCr[TexturePos - 2] != -1 && textureCr[TexturePos - 1] != -1)
					{
						//場所も色も同じ
						for (int i = 0; i < 4; i++)
						{
							//ここで今の列の四つの丸を判定しています
							if (textureCr[TexturePos - (4 - i)] == textureCr[40 + i])
							{
								ResultTexture[TexturePos - (4 - i)] = 0;
								Count++;
							}
						}
						//場所はあっていないが色は存在する
						for (int i = 0; i < 4; i++)
						{
							if (ResultTexture[TexturePos - (4 - i)] != 0)
							{
								if (textureCr[TexturePos - (4 - i)] == textureCr[40]
									&& ResultTexture[TexturePos - 4] == 2)
								{
									ResultTexture[TexturePos - 4] = 1;
								}
								else
									if (textureCr[TexturePos - (4 - i)] == textureCr[41]
										&& ResultTexture[TexturePos - 3] == 2)
									{
										ResultTexture[TexturePos - 3] = 1;
									}
									else
										if (textureCr[TexturePos - (4 - i)] == textureCr[42]
											&& ResultTexture[TexturePos - 2] == 2)
										{
											ResultTexture[TexturePos - 2] = 1;
										}
										else
											if (textureCr[TexturePos - (4 - i)] == textureCr[43]
												&& ResultTexture[TexturePos - 1] == 2)
											{
												ResultTexture[TexturePos - 1] = 1;
											}
							}

						}

						//これは4つ判定してミスがありなおかつ試行回数が１０回を
						//超えたときResultに進む
						if ( Count == 4 || HeightTexture > 9)
						{
							game = OVER;
						}
						else
						{
							Count = 0;
							HeightTexture++;
						}

					}
				}

				//ここからメイン描画
						//ここで高さ判定　いま何階層表記されるか
				for (int f = 0; f < HeightTexture; f++)
				{
					//ここで横４つの丸の色を描画していく
					for (int i = 0; i < 4; i++)
					{
						if (textureCr[splitTextureX] != -1)
						{
							//画像の中心座標 iは描画する位置に関係していて１上がるごとに右に描画する
							sprite.SetPos(Pixel * i + Pixel / 2, Pixel*(f + 1));
							//ここで表示
							textureColor.SetNum(textureCr[splitTextureX], 0);
							//スプライトの描画
							sprite.Draw(textureColor);

							//ここで答えをする場所の座標を指定　判定描画
							if (ResultTexture[splitTextureX] == 0)
							{
								trueCount++;
								//５個目の場所を初期値　、正解の数分横へ、描画する高さ
								sprite.SetPos(Pixel * 5 + Pixel / 2 + (Pixel* (trueCount - 1)), Pixel * (f + 1));	//画像の中心座標
								//ここで表示 正解の色描画
								textureResult.SetNum(0, 0);
								//スプライトの描画
								sprite.Draw(textureResult);

							}
							else if (ResultTexture[splitTextureX] == 1)
							{
								falseCount++;
							}
							splitTextureX++;
						}
					}
					for (int i = 0; i < falseCount; i++)
					{
						sprite.SetPos(Pixel * 5 + Pixel / 2 + Pixel* (i + trueCount), Pixel * (f + 1));	//画像の中心座標
																								//ここで表示
						textureResult.SetNum(1, 0);
						//スプライトの描画
						sprite.Draw(textureResult);
					}
					falseCount = trueCount = 0;
				}
				splitTextureX = 0;

			}
			if (game == PLAY)
			{
				//ここで答えをする場所の座標を指定　Aを描画
				sprite.SetPos(Pixel * 4 + Pixel / 2, Pixel* HeightTexture);	//画像の中心座標
				//ここで表示
				textureColor.SetNum(6, 0);
				//スプライトの描画
				sprite.Draw(textureColor);

				//ここで操作する場所の座標を指定　□を描画
				sprite.SetPos(Pixel * PlayerX + Pixel / 2, Pixel* (HeightTexture));	//画像の中心座標
				//ここで表示
				textureColor.SetNum(7, 0);
				//スプライトの描画
				sprite.Draw(textureColor);
			}

			if (game == OVER || game == START)
			{
				//ここで操作する場所の座標を指定　□を描画
				sprite2.SetPos(screenWidth / 2, screenHeight / 2);	//画像の中心座標
					
				if (game == START)
				{
					//スプライトの描画
					sprite2.Draw(textureStart);
				}
				else if( game == OVER)
				{
					if (Count == 4)
					{
						//スプライトの描画
						sprite2.Draw(textureClear);
					}
					else if(HeightTexture == 10)
					{
						//スプライトの描画
						sprite2.Draw(textureOver);
					}
				}

			}

					//答え描画
					if (game == OVER)
					{
						for (int i = 0; i < 4; i++)
						{
							//画像の中心座標 iは描画する位置に関係していて１上がるごとに右に描画する
							sprite.SetPos(Pixel * i + Pixel / 2, Pixel*(15 + 1));
							//ここで表示
							textureColor.SetNum(textureCr[40 + splitTextureX], 0);
							//スプライトの描画
							sprite.Draw(textureColor);

							splitTextureX++;
						}
						splitTextureX = 0;
					}
					
				
				
				//描画終了の合図
				d3d.EndScene();

				//バックバッファをフロントへ反映
				d3d.Present();
			}
		
	}
	return 0;
}
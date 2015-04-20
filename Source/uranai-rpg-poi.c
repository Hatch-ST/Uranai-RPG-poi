#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

//フィールドのサイズ
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 8
//フィールド表示用の枠のサイズ
#define FRAME_WIDTH FIELD_WIDTH*2+1
#define FRAME_HEIGHT FIELD_HEIGHT*2+1
//プレイヤー名の最大文字数
#define MAX_NAME_LENGTH 20
//敵一種類の配置数
#define ONE_ENEMY_NUMBER 5
//敵全部の配置数
#define ALL_ENEMY_NUMBER ONE_ENEMY_NUMBER*3+1
//スライムの体力,攻撃力
#define SLIME_STAMINA 20
#define SLIME_ATTACK 4
//ゴブリンの体力,攻撃力
#define GOBLIN_STAMINA 15
#define GOBLIN_ATTACK 5
//ハーピーの体力,攻撃力
#define HARPY_STAMINA 18
#define HARPY_ATTACK 6
//ガーゴイルの体力,攻撃力
#define GARGOYLE_STAMINA 50
#define GARGOYLE_ATTACK 8
//薬草の回復ポイント
#define HEAL_POINT 10
//キーボードのボタン番号
#define ESC_BUTTON 0x1b
#define ENTER_BUTTON 0x0d
#define UP_BUTTON 0x48				//↑キー
#define DOWN_BUTTON 0x50			//↓キー
#define LEFT_BUTTON 0x4B			//←キー
#define RIGHT_BUTTON 0x4D			//→キー




//ゲーム画面管理用定数
enum GAME_STATE {START,MAP,BATTLE,CLEAR,GAMEOVER,END};
//敵の移動タイプ
enum MOVE_TYPE {STOP,PURSUE,RANDOM};
//アイテム番号
enum ITEM_NUMBER {NOTHING,HEAL_ITEM,POWER_ITEM};
//生きているか死んでいるか
enum ENEMY_STATE {ALIVE,DEAD};
//移動方向
enum WAY{UP,DOWN,LEFT,RIGHT};
//敵の表示
enum ENEMY_SEE{VISIBLE,INVISIBLE};


//プレイヤーの構造体
typedef struct gPlayer{
	char name[MAX_NAME_LENGTH];			//名前
	int stamina;						//体力
	int attack;							//攻撃力
	int x;								//X座標
	int y;								//Y座標
	int firstStamina;					//初期体力
}gPlayer;

//敵の構造体
typedef struct gEnemy{
	char name[MAX_NAME_LENGTH];			//名前
	int stamina;						//体力
	int attack;							//攻撃力
	int x;								//X座標
	int y;								//Y座標
	int moveType;						//動き方
	int item;							//所持アイテム
	int isSurvived;						//生きているかどうか
	char icon[3];						//表示する文字
}gEnemy;


//プロトタイプ宣言
void displayNameInput();
void setStrNumber();
int checkInputStr(char str[MAX_NAME_LENGTH],int length);
void setPlayerState(int nameLength);
int getStrNumber(char c);
void drawMap();
void setEnemy();
void setEnemyCoord();
int displayPlayingMap();
void displayCharcter();
int movePlayer();
int checkHitEnemy();
void displayPlayerData();
void displayEnemyData(int enemyNumber);
void displayPlayingBattle(int enemyNumber);
int setPlayerAttack(char str[100],int attackLength);
void waitPressedEsc();
void waitPressedEnter();
void displayGameover();
void displayClear();
int checkEnemyExist(int enemy_x,int enemy_y);
void movePursueEnemy();
void displayOperation();
void moveEnemy(int enemyNumber,int moveWay);
void moveRandomEnemy();



//グローバル変数
gPlayer gMyPlayer;						//プレイヤーの情報を格納する構造体
int gStrNumber[36];						//各文字の数字を格納する配列(0~10,a~zの順番)
gEnemy gEnemySet[ALL_ENEMY_NUMBER];		//敵の情報を格納する構造体の配列
int gGameState = START;					//ゲーム画面管理変数
int gEnemyState = INVISIBLE;			//敵の見れるかのフラグ

int main(){
	int enemyNumber;				//敵の番号

	//乱数の初期化
	srand((unsigned)time(NULL));
	//文字に数字を割り当てる
	setStrNumber();
	//敵を初期設定する
	setEnemy();
	//敵の座標をセットする
	setEnemyCoord();

	//ゲームのメインループ
	while(gGameState != END){
		//画面をクリアする
		system("cls");
		//ゲーム画面による分岐
		switch(gGameState){
			//スタート画面
			case START:
				displayNameInput();
				break;
			case MAP:
				enemyNumber = displayPlayingMap();
				break;
			case BATTLE:
				displayPlayingBattle(enemyNumber);
				break;
			case GAMEOVER:
				displayGameover();
				break;
			case CLEAR:
				displayClear();
				break;
		}
	}
	
}

//名前入力画面を表示する関数
void displayNameInput(){
	char input[100];					//入力用変数
	int length;							//文字数
	int inputChecker;					//入力した文字が有効かチェックするフラグ変数


	//入力ループ
	do{
		//画面をクリアする
		system("cls");
		//フラグの初期化
		inputChecker = 1;
		printf("プレイヤーの名前を入力してください\n");
		printf("※半角英数字20文字まで\n");
		//名前入力ループ
		do{
			printf(">");
			gets(input);
			length = strlen(input);
			if(length > MAX_NAME_LENGTH){
				printf("入力文字数が多すぎます\n");
			}else if(length == 0){
				printf("何も入力されていません\n");
			}else{
				inputChecker = checkInputStr(input,length);
				if(inputChecker == 1){
					printf("無効な文字が使用されています\n");
				}
			}
		}while(inputChecker);

		strcpy(gMyPlayer.name, input);
		setPlayerState(length);

		printf("\n");

		//プレイヤー情報を表示する
		displayPlayerData();

		printf("\n\n");

		//y,n入力ループ
		do{
			printf("以上のステータスで良ければ「y」\n");
			printf("入力をやり直す場合は「n」を入力してください\n");
			printf(">");
			gets(input);
			length = strlen(input);
		}while(length != 1 || (input[0] != 'n' && input[0] != 'y'));
		printf("\n\n");
	}while(input[0] == 'n');

	//プレイヤーの初期位置をセット
	gMyPlayer.x = 0;
	gMyPlayer.y = 0;

	//ゲーム画面に遷移する
	gGameState = MAP;
}

//各文字に番号を割り当てる関数
void setStrNumber(){
	//11=a,12=b,13=c,14=d,15=e,16=f,17=g,18=h,19=i,20=j,21=k,22=l,23=m,
	//24=n,25=o,26=p,27=q,28=r,29=s,30=t,31=u,32=v,33=w,34=x,35=y,36=z

	int i;			//ループ用変数

	for(i = 0; i < 36; i++){
		if(i == 13 || i == 22){
			//lとcは2を割り当てる
			gStrNumber[i] = 2;
		}else if(i == 21 || i == 31 || i == 35){
			//uとkとyは1を割り当てる
			gStrNumber[i] = 1;
		}else{
			//その他はランダムに-1~3を割り当てる
			gStrNumber[i] = rand() % 5 - 1;
		}
	}

	//デバッグ
	/*
	for(i = 0; i < 36; i++){
		printf("gStrNumber[%d] = %d \n",i,gStrNumber[i]);
	}
	*/
}

//無効な文字列が使われていないかを判定する関数
int checkInputStr(char str[20],int length){
	int i;					//ループ用変数
	for(i = 0; i < length; i++){
		if((str[i] < '0' || '9' < str[i]) && (str[i] < 'a' || 'z' < str[i]) && (str[i] < 'A' || 'Z' < str[i])){
			//無効な文字が使われているので1を返す
			return 1;
		}
	}
	//ここまできたら全て有効の文字なので0を返す
	return 0;
}

//文字列からステータスを決定する関数
void setPlayerState(int nameLength){
	int i;							//ループ用変数
	int stamina=0;					//体力用変数
	int bonusCheker[5]={0};			//luckyが含まれているかチェックする配列

	for(i = 0; i < nameLength; i++){
		//体力を増減させる
		stamina += getStrNumber(gMyPlayer.name[i]);
		//luckyが含まれているか判定する
		switch(gMyPlayer.name[i]){
			case 'l':
				bonusCheker[0] = 1;
				break;
			case 'u':
				bonusCheker[1] = 1;
				break;
			case 'c':
				bonusCheker[2] = 1;
				break;
			case 'k':
				bonusCheker[3] = 1;
				break;
			case 'y':
				bonusCheker[4] = 1;
				break;
		}
	
		//デバッグ
		//printf("stamina = %d\n",stamina);
	}

	//体力が0以下の場合
	if(stamina <= 0){
			stamina = 0;
	}else{
		//体力にレートをかける
		stamina = stamina * MAX_NAME_LENGTH / nameLength;

		//名前の文字数が７の倍数の場合
		if(nameLength % 7 == 0){
			//体力を1.2倍する
			stamina = (int)(stamina * 1.2);
		}
	}
	
	//プレイヤーの体力を決定
	gMyPlayer.stamina = stamina + 10;
	gMyPlayer.firstStamina = gMyPlayer.stamina;

	//プレイヤーの攻撃力を決定
	gMyPlayer.attack = 5;
	//名前にluckyが含まれている場合
	if(bonusCheker[0] == 1 && bonusCheker[1] == 1 && bonusCheker[2] == 1 && bonusCheker[3] == 1 && bonusCheker[4] == 1){
		//攻撃力をボーナスで１加算
		gMyPlayer.attack++;
	}

}

//その文字に割り当てられている番号を返す関数
int getStrNumber(char c){
	//0~9の場合
	if('0' <= c && c <= '9'){
		return gStrNumber[c-48];
	}
	//小文字の場合
	else if('a' <= c && c <= 'z'){
		return gStrNumber[c-54-32];
	}
	//大文字の場合
	else if('A' <= c && c <= 'Z'){
		return gStrNumber[c-54];
	}
	//無効な文字の場合
	else{
		return 0;
	}
}

//マップを表示する関数
void drawMap(){
	int width,height;					//枠の幅と高さ

	//枠の表示
	for(height = 0; height < FRAME_HEIGHT; height++){
		//width×heightでコメント
		for(width = 0; width < FRAME_WIDTH; width++){
			if(height == 0){
				//0×0
				if(width == 0)					printf("┏");
				//0×最終列
				else if(width == FRAME_WIDTH-1)	printf("┓");
				//0×偶数列
				else if(width % 2 == 0)			printf("┳");
				//0×奇数列
				else							printf("━");
			}else if(height == FRAME_HEIGHT-1){
				//最終行×0
				if(width == 0)					printf("┗");
				//最終行×最終列
				else if(width == FRAME_WIDTH-1)	printf("┛");
				//最終行×偶数列
				else if(width % 2 == 0)			printf("┻");
				//最終行×奇数列
				else							printf("━");
			}else if(height % 2 == 0){
				//偶数行×0
				if(width == 0)					printf("┣");
				//偶数行×最終列
				else if(width == FRAME_WIDTH-1)	printf("┫");
				//偶数行×偶数列
				else if(width % 2 == 0)			printf("╋");
				//偶数行×奇数列
				else							printf("━");
			}else{
				//奇数行×0
				if(width == 0){
					//行数の表示
					printf("┃");
				}
				//奇数行×偶数列
				else if(width % 2 == 0)			printf("┃");
				//奇数行×奇数列
				else{
					printf("  ");
				}
			}
		}
		//1列終了の改行
		printf("\n");
	}
}

//マップ画面を表示する関数
int displayPlayingMap(){
	int moved;										//移動完了したかのフラグ変数
	int hit;										//敵に接触したかのフラグ変数

	//カーソルの表示を消す
	CONSOLE_CURSOR_INFO dursorInfo = { 1, FALSE };
	SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &dursorInfo);


	//マップを表示する
	drawMap();
	printf("\n");
	//プレイヤー情報を表示する
	displayPlayerData();
	//操作説明を表示する
	displayOperation();
	//キャラクターを表示する
	displayCharcter();

	while(1){
		//プレイヤーの移動処理
		moved = movePlayer();
		//移動が完了した場合
		if(moved == 1){
			//敵の移動処理
			movePursueEnemy();
			moveRandomEnemy();
			//当たり判定を調べる
			hit = checkHitEnemy();
			//敵と接触した場合
			if(hit != -1){
				//バトル画面に遷移
				gGameState = BATTLE;
				//接触した敵の番号を返す
				return hit;
			}
		}else if(moved == 2){
			return -1;
		}
	}

	return -1;
}

//敵の初期設定をする関数
void setEnemy(){
	//0~4 = 動かない敵, 5~9 = 追いかける敵, 10~14 = ランダムに動く敵, 15 = ボス
	//各敵1,2番目はアイテム無し,3,4番目は回復アイテム,5番目は攻撃アップアイテム

	int i;											//ループ用変数
	//char c = 'a';
	char num = '1';
	char icon[3];
	icon[2] = '\0';

	//名前,移動タイプ,攻撃力,体力をセットする
	for(i = 0 ;i < ALL_ENEMY_NUMBER; i++){
		if(i < 5){
			strcpy(gEnemySet[i].name , "Slime");
			gEnemySet[i].moveType = STOP;
			gEnemySet[i].attack = SLIME_ATTACK;
			gEnemySet[i].stamina = SLIME_STAMINA + (rand()%3-1);
			//表示する文字をセット
			icon[0] = 'S';
			icon[1] = num;
			strcpy(gEnemySet[i].icon ,icon);
		}else if(i < 10){
			strcpy(gEnemySet[i].name , "Goblin");
			gEnemySet[i].moveType = PURSUE;
			gEnemySet[i].attack = GOBLIN_ATTACK;
			gEnemySet[i].stamina = GOBLIN_STAMINA + (rand()%3-1);
			//表示する文字をセット
			icon[0] = 'G';
			icon[1] = num;
			strcpy(gEnemySet[i].icon ,icon);
		}else if(i < 15){
			strcpy(gEnemySet[i].name , "Harpy");
			gEnemySet[i].moveType = RANDOM;
			gEnemySet[i].attack = HARPY_ATTACK;
			gEnemySet[i].stamina = HARPY_STAMINA + (rand()%3-1);
			//表示する文字をセット
			icon[0] = 'H';
			icon[1] = num;
			strcpy(gEnemySet[i].icon ,icon);
		}else{
			strcpy(gEnemySet[i].name , "Gargoyle");
			gEnemySet[i].moveType = STOP;
			gEnemySet[i].attack = GARGOYLE_ATTACK;
			gEnemySet[i].stamina = GARGOYLE_STAMINA + (rand()%3-1);
			//表示する文字をセット
			icon[0] = 'B';
			icon[1] = 's';
			strcpy(gEnemySet[i].icon ,icon);
		}
		//全てに生きている状態をセットする
		gEnemySet[i].isSurvived = ALIVE;

		if(num < '5'){ 
			num++;
		}else{
			num = '1';
		}
	}

	//アイテムをセットする
	for(i = 0 ;i < ALL_ENEMY_NUMBER; i++){
		if(i % 5 == 0 || i % 5 == 1){
			gEnemySet[i].item = NOTHING;
		}else if(i % 5 == 2 || i % 5 == 3){
			gEnemySet[i].item = HEAL_ITEM;
		}else{
			gEnemySet[i].item = POWER_ITEM;
		}
	}
}

//マップに各敵キャラの座標をセットする関数
void setEnemyCoord(){
	int map[FIELD_WIDTH][FIELD_HEIGHT]={0};	//敵配置用に用意したマップ
	int countEmpty;							//置ける場所の数
	int countEnemy;							//配置する敵の番号
	int enemyNumber = ALL_ENEMY_NUMBER-1;	//ボスを除く全敵の数
	int randomNumber;						//乱数用変数
	int x,y;								//座標
	int xTable[FIELD_WIDTH*FIELD_HEIGHT],yTable[FIELD_WIDTH*FIELD_HEIGHT];	//置ける場所を格納する配列

	//プレイヤーの座標をマップにセットする
	map[gMyPlayer.x][gMyPlayer.y] = 1;
	//ボスキャラの座標をセットする
	gEnemySet[enemyNumber].x = FIELD_WIDTH-1;
	gEnemySet[enemyNumber].y = FIELD_HEIGHT-1;
	//ボスキャラの座標をマップにセットする
	map[gEnemySet[enemyNumber].x][gEnemySet[enemyNumber].y] = 1;

	countEnemy = 0;
	//置いていない場所から敵を配置する
	while(countEnemy < enemyNumber){
		countEmpty = 0;
		for(x = 0; x < FIELD_WIDTH; x++){
			for(y = 0; y < FIELD_HEIGHT; y++){
				if(map[x][y] == 0){
					xTable[countEmpty] = x;
					yTable[countEmpty] = y;
					countEmpty++;
				}
			}
		}
		randomNumber = rand()%countEmpty;
		x = xTable[randomNumber];
		y = yTable[randomNumber];
		map[x][y] = 1;
		//敵の座標をセットする
		gEnemySet[countEnemy].x = x;
		gEnemySet[countEnemy].y = y;
		countEnemy++;
	}
}

//カーソルの位置を座標から枠に合わせて変換する関数
void setCursorPosition( const int x, const int y ){
	COORD pos;
	pos.X = x * 4 + 2;
	pos.Y = y * 2 + 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
}

//マップにキャラを表示する関数
void displayCharcter(){
	int i;										//ループ用変数

	//プレイヤーの表示
	setCursorPosition(gMyPlayer.x, gMyPlayer.y);
	printf("自");

	//敵が可視の場合
	if(gEnemyState == VISIBLE){
		//敵の表示
		for(i = 0; i < ALL_ENEMY_NUMBER; i++){
			//生きている敵のみ表示する
			if(gEnemySet[i].isSurvived == ALIVE){
				setCursorPosition(gEnemySet[i].x, gEnemySet[i].y);
				printf("%2s",gEnemySet[i].icon);
			}
		}
	}
}

//プレイヤーを移動させる関数
int movePlayer(){
	char input;
	//キーボードの入力を調べる
	if(kbhit()){
		input = getch();
		switch( input ){
			case '8' :
			case UP_BUTTON:
				if( 0 < gMyPlayer.y){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.y--;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("自");
					return 1;
				}
				break;
			case '2' :
			case DOWN_BUTTON:
				if( gMyPlayer.y < FIELD_HEIGHT-1){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.y++;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("自");
					return 1;
				}
				break;
			case '4' :
			case LEFT_BUTTON:
				if( 0 < gMyPlayer.x){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.x--;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("自");
					return 1;
				}
				break;
			case '6' :
			case RIGHT_BUTTON:
				if( gMyPlayer.x < FIELD_WIDTH-1){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.x++;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("自");
					return 1;
				}
				break;
			case ESC_BUTTON :
				//敵の可視不可視を変更
				if(gEnemyState == VISIBLE){
					gEnemyState = INVISIBLE;
				}else{
					gEnemyState = VISIBLE;
				}
				return 2;
				break;
		}
	}
	return 0;
}

//追跡する敵を移動させる関数
void movePursueEnemy(){
	int i;							//ループ用変数
	int my_x,my_y;					//プレイヤーの座標
	int enemy_x,enemy_y;			//敵の座標
	int enemyExist;					//敵が存在するかのフラグ変数
	int difference_x,difference_y;	//敵とプレイヤーの距離の差
	//プレイヤーの座標を取得
	my_x = gMyPlayer.x;
	my_y = gMyPlayer.y;

	for(i = 0; i < ONE_ENEMY_NUMBER; i++){
		//その敵が生きている場合
		if(gEnemySet[i+ONE_ENEMY_NUMBER*1].isSurvived == ALIVE){
			//敵の座標を取得
			enemy_x = gEnemySet[i+ONE_ENEMY_NUMBER*1].x;
			enemy_y = gEnemySet[i+ONE_ENEMY_NUMBER*1].y;

			//敵とプレイヤーの距離の差をとる
			//x方向の差
			if(enemy_x < my_x){
				difference_x = my_x - enemy_x;
			}else{
				difference_x = enemy_x - my_x;
			}
			//y方向の差
			if(enemy_y < my_y){
				difference_y = my_y - enemy_y;
			}else{
				difference_y = enemy_y - my_y;
			}

			//x方向のほうが差が大きい場合x方向優先
			if(difference_x > difference_y){
				//プレイヤーが敵より右にいる場合
				if(enemy_x < my_x){
					//右に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x+1,enemy_y);
					if(enemyExist == 0){
						//右へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,RIGHT);
						continue;
					}
				}
				//プレイヤーが敵より左にいる場合
				else if(enemy_x > my_x){
					//左に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x-1,enemy_y);
					if(enemyExist == 0){
						//右へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,LEFT);
						continue;
					}
				}

				//プレイヤーが敵より下にいる場合
				if(enemy_y < my_y){
					//下に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x,enemy_y+1);
					if(enemyExist == 0){
						//下へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,DOWN);
						continue;
					}
				}
				//プレイヤーが敵より上にいる場合
				else if(enemy_y > my_y){
					//上に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x,enemy_y-1);
					if(enemyExist == 0){
						//上へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,UP);
						continue;
					}
				}
			}
			//y方向のほうが差が大きい場合y方向優先
			else{
				
				//プレイヤーが敵より下にいる場合
				if(enemy_y < my_y){
					//下に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x,enemy_y+1);
					if(enemyExist == 0){
						//下へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,DOWN);
						continue;
					}
				}
				//プレイヤーが敵より上にいる場合
				else if(enemy_y > my_y){
					//上に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x,enemy_y-1);
					if(enemyExist == 0){
						//上へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,UP);
						continue;
					}
				}

				//プレイヤーが敵より右にいる場合
				if(enemy_x < my_x){
					//右に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x+1,enemy_y);
					if(enemyExist == 0){
						//右へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,RIGHT);
						continue;
					}
				}
				//プレイヤーが敵より左にいる場合
				else if(enemy_x > my_x){
					//左に敵がいるか調べる
					enemyExist = checkEnemyExist(enemy_x-1,enemy_y);
					if(enemyExist == 0){
						//右へ移動
						moveEnemy(i+ONE_ENEMY_NUMBER*1,LEFT);
						continue;
					}
				}
			}
		}
	}	
}

//ランダム移動の敵を移動させる関数
void moveRandomEnemy(){
	int i;							//ループ用変数
	int countEmpty;					//移動可能方向の個数
	int movableTable[4];			//移動可能方向
	int enemy_x,enemy_y;			//敵の座標
	int enemyExist;					//敵が存在するかのフラグ変数
	int random;						//乱数用変数
	int my_x,my_y;					//プレイヤーの座標

	//プレイヤーの座標を取得
	my_x = gMyPlayer.x;
	my_y = gMyPlayer.y;

	for(i = 0; i < ONE_ENEMY_NUMBER; i++){
		countEmpty = 0;
		//その敵が生きている場合
		if(gEnemySet[i+ONE_ENEMY_NUMBER*2].isSurvived == ALIVE){
			//敵の座標を取得
			enemy_x = gEnemySet[i+ONE_ENEMY_NUMBER*2].x;
			enemy_y = gEnemySet[i+ONE_ENEMY_NUMBER*2].y;

			//プレイヤーと座標が一致した場合は移動しない
			if(my_x == enemy_x && my_y == enemy_y){
				continue;
			}

			//上方向を調べる
			if(enemy_y != 0){
				//上に敵がいるか調べる
				enemyExist = checkEnemyExist(enemy_x,enemy_y-1);
				if(enemyExist == 0){
					movableTable[countEmpty] = UP;
					countEmpty++;
				}
			}
			//下方向を調べる
			if(enemy_y != FIELD_HEIGHT-1){
				//下に敵がいるか調べる
				enemyExist = checkEnemyExist(enemy_x,enemy_y+1);
				if(enemyExist == 0){
					movableTable[countEmpty] = DOWN;
					countEmpty++;
				}
			}
			//左方向を調べる
			if(enemy_x != 0){
				//左に敵がいるか調べる
				enemyExist = checkEnemyExist(enemy_x-1,enemy_y);
				if(enemyExist == 0){
					movableTable[countEmpty] = LEFT;
					countEmpty++;
				}
			}
			//右方向を調べる
			if(enemy_x != FIELD_WIDTH-1){
				//右に敵がいるか調べる
				enemyExist = checkEnemyExist(enemy_x+1,enemy_y);
				if(enemyExist == 0){
					movableTable[countEmpty] = RIGHT;
					countEmpty++;
				}
			}
			
			//ランダムに移動
			if(countEmpty != 0){
				random = rand() % countEmpty;
				moveEnemy(i+ONE_ENEMY_NUMBER*2,movableTable[random]);
			}
		}
	}	
}

//敵を表示に合わせて移動させる
void moveEnemy(int enemyNumber,int moveWay){
	//敵が可視の場合
	if(gEnemyState == VISIBLE){
		setCursorPosition(gEnemySet[enemyNumber].x, gEnemySet[enemyNumber].y);
		printf("  ");
	}
		
	switch(moveWay){
		case UP:
			gEnemySet[enemyNumber].y--;
			break;
		case DOWN:
			gEnemySet[enemyNumber].y++;
			break;
		case LEFT:
			gEnemySet[enemyNumber].x--;
			break;
		case RIGHT:
			gEnemySet[enemyNumber].x++;
			break;
	}
	//敵が可視の場合
	if(gEnemyState == VISIBLE){
		setCursorPosition(gEnemySet[enemyNumber].x, gEnemySet[enemyNumber].y);
		printf("%2s",gEnemySet[enemyNumber].icon);
	}
}
//敵がその座標にいるかを調べる関数
int checkEnemyExist(int enemy_x,int enemy_y){
	int i;					//ループ用変数

	for(i = 0; i < ALL_ENEMY_NUMBER; i++){
		//その敵が死んでいる場合
		if(gEnemySet[i].isSurvived == DEAD){
			continue;
		}else{
			//同じ座標の敵がいる場合
			if(enemy_x == gEnemySet[i].x && enemy_y == gEnemySet[i].y){
				//1を返す
				return 1;
			}
		}
	}
	//ここまできたら敵がいないので0を返す
	return 0;
}



//敵との当たり判定を調べる関数
int checkHitEnemy(){
	int i;					//ループ用変数
	int my_x,my_y;			//自キャラの座標

	//プレイヤーの座標を取得する
	my_x = gMyPlayer.x;
	my_y = gMyPlayer.y;

	for(i = 0; i < ALL_ENEMY_NUMBER; i++){
		//その敵が死んでいる場合
		if(gEnemySet[i].isSurvived == DEAD){
			continue;
		}else{
			//同じ座標の敵がいる場合
			if(my_x == gEnemySet[i].x && my_y == gEnemySet[i].y){
				//その敵の番号を返す
				return i;
			}
		}
	}

	//重なっていなければ-1を返す
	return -1;
}

//プレイヤー情報を表示する関数
void displayPlayerData(){
	printf("プレイヤー名：%s\n",gMyPlayer.name);
	printf("　　体力　　：%d\n",gMyPlayer.stamina);
	printf("　 攻撃力 　：%d\n",gMyPlayer.attack);
}
//敵のステータスを表示する
void displayEnemyData(int enemyNumber){
	printf("　　名前　　：%s\n",gEnemySet[enemyNumber].name);
	printf("　　体力　　：%d\n",gEnemySet[enemyNumber].stamina);
	printf("　 攻撃力 　：%d\n",gEnemySet[enemyNumber].attack);
}

//バトル画面を表示する関数
void displayPlayingBattle(int enemyNumber){
	int damagePoint;						//ダメージと回復ポイント
	char input[100];						//入力用変数
	int random;								//敵の攻撃用変数

	printf("%sが現れた!!",gEnemySet[enemyNumber].name);
	printf("\n\n");

	//バトルメインループ
	while(gGameState == BATTLE){
		
		//プレイヤーの情報を表示
		displayPlayerData();
		printf("\n");

		//敵のステータスを表示
		displayEnemyData(enemyNumber);
		printf("\n\n");

		//プレイヤーの攻撃
		printf("%sの攻撃!(攻撃分の文字を入力しよう)\n",gMyPlayer.name);
		printf(">");
		gets(input);
		//ダメージ計算
		damagePoint = setPlayerAttack(input,gMyPlayer.attack);
		if(damagePoint == 0){
			printf("ミス!!ダメージを与えられなかった!!\n");
		}else{
			printf("%sに%dのダメージを与えた!!\n",gEnemySet[enemyNumber].name,damagePoint);
			//体力減算
			gEnemySet[enemyNumber].stamina -= damagePoint;
			//体力確認
			if(gEnemySet[enemyNumber].stamina <= 0){
				printf("%sを倒した!!\n",gEnemySet[enemyNumber].name);
				gEnemySet[enemyNumber].isSurvived = DEAD;
				//ボス撃破の確認
				if(enemyNumber == ALL_ENEMY_NUMBER-1){
					//クリア画面に遷移
					gGameState= CLEAR;
				}else{
					//アイテム確認
					switch (gEnemySet[enemyNumber].item){
						case NOTHING:
							printf("%sは何も持っていなかった…\n",gEnemySet[enemyNumber].name);
							break;
						case HEAL_ITEM:
							printf("%sは薬草を持っていた!!\n",gEnemySet[enemyNumber].name);
							//回復するポイントを計算
							damagePoint = HEAL_POINT + (rand()%5-2);
							printf("%sは%d回復した!!\n",gMyPlayer.name,damagePoint);
							gMyPlayer.stamina += damagePoint;
							break;
						case POWER_ITEM:
							printf("%sは力の種を持っていた!!\n",gEnemySet[enemyNumber].name);
							printf("%sの攻撃力が1上がった!!\n",gMyPlayer.name);
							gMyPlayer.attack++;
							break;
					}
					//マップ画面に遷移
					gGameState = MAP;
				}
			}
		}

		printf("\n\n");
		
		//敵が生きている場合
		if(gEnemySet[enemyNumber].isSurvived == ALIVE){
			//敵の攻撃
			printf("%sの攻撃!!",gEnemySet[enemyNumber].name);
			random = rand() % 100;
			//25%の確率でミス
			if(0 <= random && random < 25){
				printf("ミス!!ダメージを与えられなかった!!\n");
				damagePoint = 0;
			}else{
				//65%の確率で通常ダメージ
				if(25 <= random && random < 90){
					//ダメージを計算
					damagePoint = gEnemySet[enemyNumber].attack+ (rand()%3-1);
					printf("%sに%dのダメージを与えた!!\n",gMyPlayer.name,damagePoint);
				}
				//10%の確率でクリティカル
				else{
					//ダメージを計算
					damagePoint = gEnemySet[enemyNumber].attack+2;
					printf("クリティカルヒット!!%sに%dのダメージを与えた!!\n",gMyPlayer.name,damagePoint);
				}
				//体力減算
				gMyPlayer.stamina -= damagePoint;
				//体力確認
				if(gMyPlayer.stamina <= 0){
					printf("%sはやられてしまった…\n",gMyPlayer.name);
					//ゲームオーバー画面に遷移
					gGameState = GAMEOVER;
				}
			}
		}

		printf("\n");
		printf("「Enter」で次に進みます。");
		//確認を待つ
		waitPressedEnter();

		//画面をクリアする
		system("cls");
	}
}

//プレイヤーが与えるダメージを決定する関数
int setPlayerAttack(char str[100],int attackLength){
	int i;							//ループ用変数
	int damagePoint = 0;			//ダメージ

	for(i = 0; i < attackLength; i++){
		//体力を増減させる
		damagePoint += getStrNumber(str[i]);
	}

	//0以下の場合は0にする
	if(damagePoint < 0){
		damagePoint = 0;
	}

	return damagePoint;
}

//ゲームオーバー画面を表示する
void displayGameover(){
	printf("その後%sを知るものはいなかった…\n\n",gMyPlayer.name);
	//ゲームを終了する
	gGameState = END;
}

//クリア画面を表示する
void displayClear(){
	int staminaPoint;		//体力ポイント
	int attackPoint;		//攻撃力ポイント
	int luckyPoint;			//運勢ポイント
	printf("「Enter」で次に進みます。\n\n");
	printf("クリアおめでとうございます!!\n");
	printf("最後に%sさんの運勢を占いましょう!\n\n",gMyPlayer.name);
	//確認を待つ
	waitPressedEnter();

	//残り体力-初期体力
	staminaPoint = gMyPlayer.stamina - gMyPlayer.firstStamina;
	//攻撃力×2
	attackPoint = gMyPlayer.attack * 2;
	//運勢ポイントの計算
	luckyPoint = staminaPoint + attackPoint;

	printf("あなたのラッキーポイントは%dです\n\n",luckyPoint);

	//確認を待つ
	waitPressedEnter();

	//運勢の表示
	if(10 < luckyPoint ){
		printf("なんてあなたは運がいいんでしょうか!!\n");
		printf("きっとこの後も素敵なことがあることでしょう!!\n");	
	}else if(-10 < luckyPoint){
		printf("そこそこといったところでしょうか。\n");
		printf("いつも通りな１日になるでしょうね。\n");	
	}else{
		printf("おやおや散々でしたか？\n");
		printf("でもクリアできただけ良いことがありますよ。\n");	
	}

	printf("お疲れ様でした。「Esc」で終了します\n");
	//確認を待つ
	waitPressedEsc();

	//ゲームを終了する
	gGameState = END;
}

//ESCを押すまで待つ関数
void waitPressedEsc(){
	char input;			//入力したキー
	
	//確認を待つループ
	while(1){
		if(kbhit()){
			input = getch();
			if(input == ESC_BUTTON){
				break;
			}
		}
	}
}

//Enterを押すまで待つ関数
void waitPressedEnter(){
	char input;			//入力したキー
	
	//確認を待つループ
	while(1){
		if(kbhit()){
			input = getch();
			if(input == ENTER_BUTTON){
				break;
			}
		}
	}
}

//操作方法を表示する関数
void displayOperation(){
	COORD pos;					//座標

	pos.X = 21 * 2 + 2;
	pos.Y = 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("操作方法");

	pos.X = 21 * 2 + 1 + 4;
	pos.Y = 2;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("↑:８");

	pos.X = 21 * 2 + 1;
	pos.Y = 3;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("←:４　→：６");

	pos.X = 21 * 2 + 1 + 4;
	pos.Y = 4;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("↓:２");

	pos.X = 21 * 2 + 1 + 1;
	pos.Y = 5;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("Esc:敵の可視切り替え");
}





	










	
	

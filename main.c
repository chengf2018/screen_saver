#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

// #include <stdio.h>

typedef struct {
    int x;
    int y;
    char str[3];
} WORD_GRID_T;

typedef struct {
    int y;
} WORD_RAIN_T;

#define RAIN_LENGTH 16
#define CHAR_HEIGHT 16 
#define GRID_WIDTH (CHAR_HEIGHT + 10)
#define IS_ASCII(c) ((unsigned char)(c) < 128)
#define TEXT_COUNT (sizeof(g_context)/sizeof(char*))
#define COLOR_RATE 255 / RAIN_LENGTH

static int        g_screenW, g_screenH, g_xCount, g_yCount;
static HBITMAP    g_memmap;
static HDC        g_memdc;
static HBRUSH     g_bkbrush;
static char       **g_text;//gbk编码，每个汉字占2个字节，数字、字母及英文符号1个字节
static char       *g_context[] = {
    "既已背影倾天下，何必转身乱芳华。",
    "只要努力的时间足够长，搞不好，你还可以失败的更彻底。",
    "有时候，你最想得到的人，其实是你最应该离开的人。",
    "你的戏，可以像你的钱一样，少一点吗？",
    "你要是过得好，我怎么能睡得着。",
    "没有什么事情能把人一次击倒，只要足够坚强，它会持续的把你击倒",
    //data from "https://tool.lu/timestamp/"
    "不经巨大的困难，不会有伟大的事业。",
    "苦难磨炼一些人，也毁灭另一些人。",
    "改变你的想法，你就改变了自己的世界。",
    "不要等待，时机永远不会恰到好处。",
    "生命如同寓言，其价值不在与长短，而在与内容。",
    "你相信什么，你就成为什么样的人。",
    "生命不可能有两次，但许多人连一次也不善于度过。",
    "人的一生是短的，但如果卑劣地过这一生，就太长了。",
    "我的努力求学没有得到别的好处，只不过是愈来愈发觉自己的无知。",
    "生活的道路一旦选定，就要勇敢地走到底，决不回头。",
    "生命是一条艰险的峡谷，只有勇敢的人才能通过。",
    "要么你主宰生活，要么你被生活主宰。",
    "不幸可能成为通向幸福的桥梁。",
    "人生就是学校。在那里，与其说好的教师是幸福，不如说好的教师是不幸。",
    "接受挑战，就可以享受胜利的喜悦。",
    "节制使快乐增加并使享受加强。",
    "今天应做的事没有做，明天再早也是耽误了。",
    "决定一个人的一生，以及整个命运的，只是一瞬之间。",
    "一个不注意小事情的人，永远不会成就大事业。",
    "浪费时间是一桩大罪过。",
    "既然我已经踏上这条道路，那么，任何东西都不应妨碍我沿着这条路走下去。",
    "教育需要花费钱，而无知也是一样。",
    "坚持意志伟大的事业需要始终不渝的精神。",
    "你活了多少岁不算什么，重要的是你是如何度过这些岁月的。",
    "内外相应，言行相称。",
    "你热爱生命吗？那么别浪费时间，因为时间是组成生命的材料。",
    "坚强的信心，能使平凡的人做出惊人的事业。",
    "读一切好书，就是和许多高尚的人谈话。",
    "真正的人生，只有在经过艰难卓绝的斗争之后才能实现。",
    "伟大的事业，需要决心，能力，组织和责任感。",
    "没有人事先了解自己到底有多大的力量，直到他试过以后才知道。",
    "敢于浪费哪怕一个钟头时间的人，说明他还不懂得珍惜生命的全部价值。",
    "感激每一个新的挑战，因为它会锻造你的意志和品格。",
    "共同的事业，共同的斗争，可以使人们产生忍受一切的力量。　",
    "古之立大事者，不惟有超世之才，亦必有坚忍不拔之志。",
    "故立志者，为学之心也；为学者，立志之事也。",
    "读一本好书，就如同和一个高尚的人在交谈。",
    "学习是劳动，是充满思想的劳动。",
    "好的书籍是最贵重的珍宝。",
    "读书是易事，思索是难事，但两者缺一，便全无用处。",
    "读书是在别人思想的帮助下，建立起自己的思想。",
    "合理安排时间，就等于节约时间。",
    "你想成为幸福的人吗？但愿你首先学会吃得起苦。",
    "抛弃时间的人，时间也抛弃他。",
    "普通人只想到如何度过时间，有才能的人设法利用时间。",
    "一次失败，只是证明我们成功的决心还够坚强。",
    "取得成就时坚持不懈，要比遭到失败时顽强不屈更重要。",
    "人的一生是短的，但如果卑劣地过这一生，就太长了。",
    "失败是坚忍的最后考验。",
    "不要回避苦恼和困难，挺起身来向它挑战，进而克服它。",
    "那脑袋里的智慧，就像打火石里的火花一样，不去打它是不肯出来的。",
    "最困难的事情就是认识自己。",
    "有勇气承担命运这才是英雄好汉。",
    "最灵繁的人也看不见自己的背脊。",
    "阅读使人充实，会谈使人敏捷，写作使人精确。",
    "最大的骄傲于最大的自卑都表示心灵的最软弱无力。",
    "自知之明是最难得的知识。",
    "勇气通往天堂，怯懦通往地狱。",
    "有时候读书是一种巧妙地避开思考的方法。",
    "阅读一切好书如同和过去最杰出的人谈话。",
    "越是没有本领的就越加自命不凡。",
    "越是无能的人，越喜欢挑剔别人的错儿。",
    "知人者智，自知者明。胜人者有力，自胜者强。",
    "意志坚强的人能把世界放在手中像泥块一样任意揉捏。",
    "最具挑战性的挑战莫过于提升自我。",
    "失败也是我需要的，它和成功对我一样有价值。",
    "一个人即使已登上顶峰，也仍要自强不息。",
    "最大的挑战和突破在于用人，而用人最大的突破在于信任人。",
    "自己活着，就是为了使别人过得更美好。",
    "要掌握书，莫被书掌握；要为生而读，莫为读而生。",
    "要知道对好事的称颂过于夸大，也会招来人们的反感轻蔑和嫉妒。",
    "谁和我一样用功，谁就会和我一样成功。",
    "一切节省，归根到底都归结为时间的节省。",
    "意志命运往往背道而驰，决心到最后会全部推倒。",
    "过去一切时代的精华尽在书中。",
    "深窥自己的心，而后发觉一切的奇迹在你自己。",
    "只有把抱怨环境的心情，化为上进的力量，才是成功的保证。",
    "知之者不如好之者，好之者不如乐之者。",
    "勇猛、大胆和坚定的决心能够抵得上武器的精良。",
    "意志是一个强壮的盲人，倚靠在明眼的跛子肩上。",
    "只有永远躺在泥坑里的人，才不会再掉进坑里。",
    "希望的灯一旦熄灭，生活刹那间变成了一片黑暗。",
    "要成功不需要什么特别的才能，只要把你能做的小事做得好就行了。",
    "形成天才的决定因素应该是勤奋。",
    "学到很多东西的诀窍，就是一下子不要学很多。",
    "自己的鞋子，自己知道紧在哪里。",
    "我们唯一不会改正的缺点是软弱。",
    "我这个人走得很慢，但是我从不后退。",
    "勿问成功的秘诀为何，且尽全力做你应该做的事吧。",
    "对于不屈不挠的人来说，没有失败这回事。",
    "学问是异常珍贵的东西，从任何源泉吸收都不可耻。",
    "坚强的信念能赢得强者的心，并使他们变得更坚强。 ",
    "一个人几乎可以在任何他怀有无限热忱的事情上成功。 ",
    "卓越的人一大优点是：在不利与艰难的遭遇里百折不饶。",
    "本来无望的事，大胆尝试，往往能成功。",
    "我们若已接受最坏的，就再没有什么损失。",
    "只有在人群中间，才能认识自己。",
    "书籍把我们引入最美好的社会，使我们认识各个时代的伟大智者。",
    "当一个人用工作去迎接光明，光明很快就会来照耀着他。",
    "如果你能做梦，你就能实现它。"
    //data from http://m.3chongmen.com/mingrenmingyan/233.html
};
static WORD_GRID_T *g_gridmap;
static WORD_RAIN_T *g_rain;

static int GetGBKCharCount(const char* str) {
    int len, cnt, i;
    len = strlen(str);
    cnt = 0;
    for (i = 0; i < len; ++i) {
        ++cnt;
        if (!IS_ASCII(str[i]))
            ++i;
    }
    return cnt;
}

static const char *CopySingleWord(const char *source, char *desc) {
    int i = 0;
    if (*source == 0) return source;
    if (IS_ASCII(*source)) {
        desc[i++] = *source++;
    } else {
        desc[i++] = *source++;
        assert(*source != 0);
        desc[i++] = *source++;
    }
    desc[i] = 0;
    return source;
}

static void SetGridMapText(const char *str, int x) {
    int start, end;
    WORD_GRID_T *grid;
    start = x * g_yCount;
    end = (x + 1) * g_yCount;
    if (start < 0 || end >= g_xCount * g_yCount) return;
    for (; start < end; start++) {
        grid = &g_gridmap[start];
        if (*str == 0)
            grid->str[0] = 0;
        else
            str = CopySingleWord(str, grid->str);
    }
}

static char* UTF8ToGBK(const char* utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    if (len == 0) return NULL;

    wchar_t wstr[len * sizeof(wchar_t)];
    if (!MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len))
        return NULL;

    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (len == 0) 
        return NULL;

    char* str = malloc(len);
    if (!WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL))
    {
        free(str);
        return NULL;
    }

    return str;
}

static void CreateMemoryDC(HWND hWnd) {
    HDC hdc;

    hdc = GetDC(hWnd);
    g_memmap = CreateCompatibleBitmap(hdc, g_screenW, g_screenH);
    g_memdc = CreateCompatibleDC(hdc);
    SelectObject(g_memdc, g_memmap);
    SetBkMode(g_memdc, TRANSPARENT);
    /*SelectObject(g_memdc,
        CreateFont(CHAR_HEIGHT,0,0,0,FW_BOLD,
            FALSE,FALSE,FALSE,
            OEM_CHARSET,
            OUT_RASTER_PRECIS,
            CLIP_DEFAULT_PRECIS,
            VARIABLE_PITCH|PROOF_QUALITY,
            FF_DONTCARE,"Fixedsys")
    );*/

    ReleaseDC(hWnd, hdc);
}

static void Init() {
    int i, xCount, yCount, gridSize, gridCount;
    WORD_GRID_T *grid;
    char *str, *nstr;
    int len, blen;

    srand(time(0));
    g_screenW = GetSystemMetrics(SM_CXSCREEN);
    g_screenH = GetSystemMetrics(SM_CYSCREEN);
    xCount = g_screenW / GRID_WIDTH;
    yCount = g_screenH / GRID_WIDTH;
    g_xCount = xCount;
    g_yCount = yCount;
    g_rain = malloc(sizeof(WORD_RAIN_T) * xCount);
    g_text = malloc(sizeof(g_context));
    if (!g_rain || !g_text) return;

    for (i=0; i<TEXT_COUNT; i++) {
        str = UTF8ToGBK(g_context[i]);
        len = GetGBKCharCount(str);
        while (len < g_yCount) {
            blen = strlen(str);
            nstr = malloc(blen*2 + 1);
            strcpy(nstr, str);
            strcat(nstr, str);
            free(str);
            str = nstr;
            len = GetGBKCharCount(str);
        }
        g_text[i] = str;
    }

    gridCount = xCount * yCount;
    gridSize = sizeof(WORD_GRID_T) * gridCount;
    g_gridmap = malloc(gridSize);
    memset(g_gridmap, 0, gridSize);

    for (i=0; i<gridCount; i++) {
        grid = &g_gridmap[i];
        grid->x = (int)(i / yCount) * GRID_WIDTH;
        grid->y = (i % yCount) * GRID_WIDTH;
    }

    for (i=0; i<xCount; i++) {
        SetGridMapText(g_text[rand()%TEXT_COUNT], i);
        g_rain[i].y = -1;
    }

}

static void Deinit() {
    int i;
    for (i=0; i<TEXT_COUNT; i++) {
        free(g_text[i]);
    }
    free(g_text);
    free(g_gridmap);
}

static void DrawInit(HWND hWnd) {
    ShowCursor(FALSE);
    CreateMemoryDC(hWnd);
    g_bkbrush = CreateSolidBrush(RGB(0,0,0));
    SetTimer(hWnd, 2, 50, NULL);
}

static void DrawRelease() {
    ShowCursor(TRUE);
    DeleteObject(g_memmap);
    DeleteDC(g_memdc);
    DeleteObject(g_bkbrush);
}

static void DrawMem() {
    int x, y, rainMax, rainY;
    int color;
    WORD_GRID_T *grid;

    rainMax = g_yCount + RAIN_LENGTH;

    for (x=0; x<g_xCount; x++) {
        if (g_rain[x].y == -1) {
            if (rand()%10==0) 
                g_rain[x].y = 0;
        } else {
            g_rain[x].y ++;
            if (g_rain[x].y > g_yCount + RAIN_LENGTH) {
                g_rain[x].y = -1;
                SetGridMapText(g_text[rand()%TEXT_COUNT], x);
            }
        }
    }

    RECT rect = {0,0,g_screenW,g_screenH};
    FillRect(g_memdc, &rect, g_bkbrush);

    for (x=0; x<g_xCount; x++) {
        rainY = g_rain[x].y;
        if (rainY >= 0 && rainY < rainMax) {
            for (y=(rainY>g_yCount-1)?(g_yCount-1):rainY; y>=rainY-RAIN_LENGTH && y >= 0; y--) {
                grid = &g_gridmap[x*g_yCount+y];
                assert(grid);
                color = 255 - (rainY-y) * COLOR_RATE;
                SetTextColor(g_memdc, RGB(0, color, 0));
                TextOut(g_memdc, grid->x, grid->y, grid->str, strlen(grid->str));
            }
        }
    }
}

static void DrawMain(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    BitBlt(hdc, 0, 0, g_screenW, g_screenH, g_memdc, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hdc);
}

static void OnLogicTimer(HWND hWnd, int timerId) {
    DrawMem();
    DrawMain(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_MOUSEMOVE:
        break;
        case WM_TIMER:
            OnLogicTimer(hWnd, wParam);
        break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_CHAR:
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
    WNDCLASS wc;
    HWND hWnd;
    MSG msg;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDC_ICON);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "MyScreenSaverClass";
    if(!RegisterClass(&wc))
        return 0;

    Init();
    hWnd = CreateWindow(
        "MyScreenSaverClass",
        "My Screen Saver",
        WS_POPUP, 
        0,
        0,
        g_screenW,
        g_screenH,
        NULL,
        NULL,
        hInst,
        NULL
    );

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    DrawInit(hWnd);

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DrawRelease();
    Deinit();
    return (int) msg.wParam;
}
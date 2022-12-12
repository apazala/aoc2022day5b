#include <cstdio>
#include <vector>
#include <cstring>

using std::vector;

int loadInputFile();
int nextLine(char *dst);

char line[128];
int readCratesRaw(char cratesText[][48])
{
    int numRows = 0;
    for (int len = nextLine(line); len > 0; len = nextLine(line))
    {
        strncpy(cratesText[numRows], line, len);
        numRows++;
    }

    return numRows;
}

// Only works with single digit columns numbers in the last row!!!!!
void fillCratesStacks(vector<vector<char>> &cratesStacks, char cratesText[][48], int numRows)
{
    int lastRow = numRows - 1;
    int j = 0;
    for (char c = cratesText[lastRow][j]; c != '\0'; j++, c = cratesText[lastRow][j])
    {
        if (c >= '1')
        {
            cratesStacks.push_back(vector<char>());
            int i = lastRow-1;
            for(char crate = cratesText[i][j]; i >= 0 && crate >= 'A'; i--, crate = cratesText[i][j])
            {
                cratesStacks.back().push_back(crate);
            }
        }
    }
}

char cratesTextRaw[12][48];
int main()
{
    if (loadInputFile() < 0)
    {
        fprintf(stderr, "Can't open: input.txt\n");
        return 1;
    }

    int numRows = readCratesRaw(cratesTextRaw);

    vector<vector<char>> cratesStacks;
    fillCratesStacks(cratesStacks, cratesTextRaw, numRows);

    for (int len = nextLine(line); len > 0; len = nextLine(line))
    {
        int ncrates, colOrig, colDest;
        if(sscanf(line, "move %d from %d to %d", &ncrates, &colOrig, &colDest) != 3){
            fprintf(stderr, "move line has unexpected format!!"); //Show warn but continue
            continue;
        }

        vector<char> *stackOrig = &cratesStacks[colOrig-1];
        vector<char> *stackDest = &cratesStacks[colDest-1];
        //Faster alternative-> linked lists using nodes!
        stackDest->insert(stackDest->end(), stackOrig->end()- ncrates, stackOrig->end());
        stackOrig->erase(stackOrig->end()- ncrates, stackOrig->end());
        
    }

    for (vector<char> &stack: cratesStacks)
    {
        if(stack.size()>0){
            printf("%c", stack.back());
        }
    }
    
    printf("\n");

}

#define BUFSIZE 1 << 14

char fileBuf[BUFSIZE];
char *pbuf = fileBuf;
char *pbufend = fileBuf;

int loadInputFile()
{
    FILE *pf = fopen("input.txt", "r");
    if (!pf)
        return -1;
    pbuf = fileBuf;
    pbufend = fileBuf + fread(fileBuf, 1, BUFSIZE, pf);
    fclose(pf);

    return 0;
}

int nextLine(char *dst)
{
    char c;
    char *orig = dst;

    // read line:
    while (pbuf < pbufend && (c = *(pbuf++)) != '\n')
    {
        *(dst++) = c;
    }
    *dst = '\0';
    // return line length
    return dst - orig;
}
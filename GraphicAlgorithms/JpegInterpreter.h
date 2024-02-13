#ifndef GRAPHICEDITORUP_JPEGINTERPRETER_H
#define GRAPHICEDITORUP_JPEGINTERPRETER_H



#include <fstream>
#include <vector>

class JpegInterpreter {
private:
    class treeNode{
    public:
        int length;
        char c;
        treeNode * left;
        treeNode * right;
        treeNode * parent;
    };
    class HuffmanTable{
    public:
        char * content;
        int size;
        int type;
        int id;
        treeNode * root;
    };

    class ChanelInfo{
    public:
        int horiz_thining;
        int vertical_thining;
        int kvant_table_id;
        int id;
        treeNode * root;
        int haffman_id_dc;
        int haffman_id_ac;
    };
public:
    int width;
    int height;
    int chanelCount;
    int chtableId[3];
    std::ifstream &F;
    std::vector<unsigned char *> dqtables;
    std::vector<ChanelInfo> chanelInfos;

    std::vector<HuffmanTable> dhtablesDC;
    std::vector<HuffmanTable> dhtablesAC;
    std::vector<HuffmanTable*> dhtablesForChanels;
    std::vector<int**> imageTables;

    std::vector<treeNode *> dhtreeroots;
    unsigned char *arrBytesToShow;

    JpegInterpreter(std::ifstream &F);
    void readJpeg();
    bool readSector(char* name, char* input, int length);

private:

    void dqtDecode(char* input);
    void dhtDecode(char* input);
    void sof0Decode(char* input);
    void sosDecode(char* input);


    bool addNode(treeNode* root, int level, int curLevel, treeNode* newNode);
    void preorder(treeNode *root);
    void inorder(treeNode *root);

    int getBitfromByte (char Byte, int numOfBit);
    void dataDecode(char *input);
    void kvanting();
    void inversiveDiscreteCosineTransformation();
    void limitToColorRange();
    void finaleImageGenerate();

    void straightArrToZigZagArr(int *straight, int ** zigzag);
    void straightArrToZigZagArr(unsigned char *straight, int ** zigzag);


    double C(int u);
};


#endif //GRAPHICEDITORUP_JPEGINTERPRETER_H

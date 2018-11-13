#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <set>

enum LinelOrientation{Left,Down,Right,Up};
typedef LinelOrientation EdgeOrientation;

enum CellOrientation{Clockwise,Counterclockwise};
enum CellType{Normal,Aux,Border,Not_Defined};

struct Pixel
{
    Pixel(int row, int col,CellType ct,int varIndex):row(row),
                                                     col(col),
                                                     ct(ct),
                                                     varIndex(varIndex),
                                                     orientation(CellOrientation::Counterclockwise)
    {}

    const int row,col;
    const CellOrientation orientation;
    const CellType ct;
    const int varIndex;
};

struct Linel
{
    Linel(const Pixel& p1, const Pixel& p2, const LinelOrientation& orientation):p1(p1),
                                                                                 p2(p2),
                                                                                 orientation(orientation)
    {}

    bool operator<(const Linel& other) const
    {
        if(p1.varIndex!=other.p2.varIndex) return p1.varIndex < other.p1.varIndex;
        else return p1.orientation==LinelOrientation::Up;
    }

    const Pixel &p1,&p2;
    const LinelOrientation orientation;
};

struct Edge
{
    Edge(const Linel& l1, const EdgeOrientation& orientation, const int varIndex):l1(l1),
                                                                                  orientation(orientation),
                                                                                  varIndex(varIndex)
    {}

    const Linel &l1;
    const EdgeOrientation orientation;
    const int varIndex;
};

struct ExtendedImage
{
    ExtendedImage(const cv::Mat& image, int border):image(image),
                                                    border(border),
                                                    extW(image.cols+2*border),
                                                    extH(image.rows+2*border)
    {}

    const cv::Mat& image;
    const int border;
    const int extW;
    const int extH;
};

CellType resolveCellType(const ExtendedImage& extImage,int row,int col)
{
    int border = extImage.border;
    row -= border;
    col -= border;

    if(row<=-(border+1) || col<=-(border+1)) return CellType::Not_Defined;
    if(row>=(extImage.extH-border) || col>=(extImage.extW-border)) return CellType::Not_Defined;

    if(row<=-border || col<=-border) return CellType::Border;
    if(row>=(extImage.extH-border-1) || col>=(extImage.extW-border-1)) return CellType::Border;

    if(row<=-1 || col<=-1) return CellType::Aux;
    if(row>=extImage.image.rows || col>=extImage.image.cols) return CellType::Aux;

    return CellType::Normal;
}

void createPixelSet(std::vector<Pixel>& pxlVector,
                    const ExtendedImage& extImage)
{
    int h = extImage.extH;
    int w = extImage.extW;

    int varIndex=0;
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            CellType ct = resolveCellType(extImage,i,j);
            if(ct==CellType::Not_Defined) continue;

            pxlVector.push_back(Pixel(i,j,ct,varIndex++));
        }
    }
}

void createLinelSet(std::set<Linel>& lnlSet, const ExtendedImage& extImage, const std::vector<Pixel>& pxlVector)
{
    int h = extImage.image.rows;
    int w = extImage.image.cols;

    int extW = extImage.extW;
    int extH = extImage.extH;

    std::vector<Pixel>::const_iterator it = pxlVector.begin();

    while(it->ct==CellType::Border) ++it;
    --it;
    do
    {
        const Pixel &a1 = *(it+1);
        const Pixel &a2 = *it;

        lnlSet.insert( Linel(a1,a2,LinelOrientation::Up) );

        const Pixel &b1 = a1;
        const Pixel &b2 = *(it-extW);
        lnlSet.insert( Linel(b1,b2,LinelOrientation::Right) );

        ++it;
        while(it->ct!=CellType::Border && it!=pxlVector.end()) ++it;

    }while(it!=pxlVector.end());

}

void createEdgeVector(std::vector<Edge>& edgVector, const std::set<Linel>& lnlSet)
{
    std::set<Linel>::const_iterator it = lnlSet.begin();
    do
    {
        const Linel& linel = *it;
        const Pixel& pixel = linel.p1;
        int baseVarIndex = pixel.varIndex*4;

        if(linel.orientation==LinelOrientation::Up || linel.orientation==LinelOrientation::Down)
        {
            edgVector.push_back( Edge(linel,EdgeOrientation::Up,baseVarIndex) );
            edgVector.push_back( Edge(linel,EdgeOrientation::Down,baseVarIndex+1) );
        }
        else
        {
            edgVector.push_back( Edge(linel,EdgeOrientation::Right,baseVarIndex+2) );
            edgVector.push_back( Edge(linel,EdgeOrientation::Left,baseVarIndex+3) );
        }

        ++it;

    }while(it!=lnlSet.end());
}

int main()
{
    typedef cv::Vec3b ImagePixelType;

    cv::Mat image=cv::imread("images/cow2.jpg");
    ExtendedImage extImage(image,2);

    std::vector<Pixel> pxlVector;
    createPixelSet(pxlVector,extImage);
    assert(pxlVector.size()==extImage.extW*extImage.extH);

    std::set<Linel> lnlSet;
    createLinelSet(lnlSet,extImage,pxlVector);
    std::for_each(lnlSet.begin(),lnlSet.end(),[](const Linel& linel){ assert(linel.p1.ct==CellType::Normal); });

    std::vector<Edge> edgVector;
    createEdgeVector(edgVector,lnlSet);



    std::cout << "Hello, World!" << std::endl;
    return 0;
}
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
    Linel(const Pixel& p1,
          const Pixel& p2,
          const LinelOrientation& orientation,
          const int linelIndex):p1(p1),
                                p2(p2),
                                orientation(orientation),
                                linelIndex(linelIndex)
    {}

    bool operator<(const Linel& other) const
    {
        return this->linelIndex < other.linelIndex;
    }

    const Pixel &p1,&p2;
    const LinelOrientation orientation;
    const int linelIndex;
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
    int linelIndex=0;

    while(it->ct!=CellType::Normal) ++it;
    --it;
    do
    {
        const Pixel &a1 = *(it+1);
        const Pixel &a2 = *it;

        lnlSet.insert( Linel(a1,a2,LinelOrientation::Up,linelIndex++) );

        const Pixel &b1 = a1;
        const Pixel &b2 = *(it-extW);
        lnlSet.insert( Linel(b1,b2,LinelOrientation::Right,linelIndex++) );

        ++it;
        while(it->ct==CellType::Border && it!=pxlVector.end()) ++it;

    }while(it!=pxlVector.end());

    assert(linelIndex==lnlSet.size());

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

typedef std::vector< int > CoefLine;
typedef std::vector< CoefLine > CoefMatrix;

void initCoefMatrix(CoefMatrix& cm,
                    const std::vector<Pixel>& pxlVector,
                    const std::set<Linel>& lnlSet,
                    const std::vector<Edge>& edgVector)
{
    int numVar = pxlVector.size()+edgVector.size();
    cm.resize(lnlSet.size());
    for(auto it=lnlSet.begin();it!=lnlSet.end();++it)
    {
        cm[it->linelIndex].resize(numVar,0);
    }
}

void setIncidence(int& coef, const CellOrientation& co, const LinelOrientation& lo, bool p1)
{
    if(lo==LinelOrientation::Left || lo==LinelOrientation::Down) coef=1;
    else coef=-1;

    if(co==Clockwise) coef=-coef;
    if(!p1) coef=-coef;
}

void setIncidence(int& coef, const EdgeOrientation& eo, const LinelOrientation& lo)
{
    if(lo==LinelOrientation::Left || lo==LinelOrientation::Right)
        assert(eo==EdgeOrientation::Left || eo==EdgeOrientation::Right);
    else
        assert(eo==EdgeOrientation::Up || eo==EdgeOrientation::Down);

    coef = eo==lo?1:-1;

}

void setPixelIncidence(CoefMatrix& cm,
                       const std::vector<Pixel>& pxlVector,
                       const std::set<Linel>& lnlSet)
{

    for(auto it=lnlSet.begin();it!=lnlSet.end();++it)
    {
        int &c1 = cm[it->linelIndex][it->p1.varIndex];
        setIncidence(c1,it->p1.orientation,it->orientation,true);

        int &c2 = cm[it->linelIndex][it->p2.varIndex];
        setIncidence(c2,it->p2.orientation,it->orientation,false);
    }
}

void setEdgeIncidence(CoefMatrix& cm,
                      const std::vector<Edge>& edgVector,
                      const std::set<Linel>& lnlSet)
{
    for(auto it=edgVector.begin();it!=edgVector.end();++it)
    {
        int &coef = cm[it->l1.linelIndex][it->varIndex];
        setIncidence(coef,it->orientation,it->l1.orientation);
    }
}

void ballFilter(std::vector<int>& leaps,int radius, const ExtendedImage& extImage)
{
    std::vector<std::pair<int,int> > coords;
    for(int i=-radius;i<=radius;++i)
    {
        for(int j=-radius;j<=radius;++j)
        {
            if(i*i+j*j<=radius*radius)
            {
                coords.push_back(std::pair<int,int>(i,j));
            }
        }
    }

    for(auto it=coords.begin();it!=coords.end();++it)
    {
        leaps.push_back(it->first*extImage.extW+it->second);
    }
}

void consistenceConstraints(std::ofstream& ofs, const CoefMatrix& cm)
{

}


int main()
{
    typedef cv::Vec3b ImagePixelType;

    cv::Mat image=cv::imread("../images/single_square.pgm");
    ExtendedImage extImage(image,2);

    std::cout << image.cols << " " << image.rows << " \n";

    std::vector<Pixel> pxlVector;
    createPixelSet(pxlVector,extImage);
    assert(pxlVector.size()==extImage.extW*extImage.extH);

    std::set<Linel> lnlSet;
    createLinelSet(lnlSet,extImage,pxlVector);
    std::for_each(lnlSet.begin(),lnlSet.end(),[](const Linel& linel){ assert(linel.p1.ct!=CellType::Not_Defined); });

    std::vector<Edge> edgVector;
    createEdgeVector(edgVector,lnlSet);


    CoefMatrix cm;
    initCoefMatrix(cm,pxlVector,lnlSet,edgVector);

    setPixelIncidence(cm,pxlVector,lnlSet);
    setEdgeIncidence(cm,edgVector,lnlSet);

    std::vector<int> leaps;
    ballFilter(leaps,3,extImage);std::cout<< extImage.extW << std::endl;
    std::for_each(leaps.begin(),leaps.end(), [](int x){ std::cout << x << std::endl;});


    consistenceConstraints();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
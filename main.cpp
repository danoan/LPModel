#include <iostream>

#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/shapes/Shapes.h"

#include "DIPaCUS/base/Representation.h"
#include "DIPaCUS/components/Transform.h"
#include "DIPaCUS/components/Morphology.h"
#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC/Core/ODRUtils.h"
#include "SCaBOliC/Core/ODRPixels.h"

typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;

typedef DGtal::Z2i::Space Space;
typedef DGtal::NGon2D<Space> NGon2D;

typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate> FourDigitalBoundary;
typedef SCaBOliC::Core::ODRPixels ODRPixels;

DigitalSet square()
{
    NGon2D square(0,0,10,4,3.1416/4.0);
    DGtal::GaussDigitizer<Space,NGon2D> gd;

    gd.init(square.getLowerBound(),square.getUpperBound(),1.0);
    gd.attach(square);

    Domain domain(square.getLowerBound(),square.getUpperBound());
    DigitalSet ds(domain);
    DGtal::Shapes<Domain>::digitalShaper(ds,gd);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}

ODRModel odrModel(const DigitalSet& originalDS)
{
    const Domain& domain = originalDS.domain();
    DigitalSet boundary(domain);
    DigitalSet optRegion(domain);

    ODRPixels odrPixels(ODRModel::AC_PIXEL,ODRModel::CM_PIXEL,3,ODRModel::FourNeighborhood);
    ODRModel odrModel = odrPixels.createODR(ODRModel::OM_OriginalBoundary,ODRModel::AM_AroundBoundary,3,originalDS);

    DigitalSet optRegionDS = odrModel.optRegion;
    optRegionDS.insert(odrModel.applicationRegion.begin(),odrModel.applicationRegion.end());

    return ODRModel(odrModel.domain,
                    odrModel.original,
                    optRegionDS,odrModel.trustFRG,
                    odrModel.trustBKG,
                    odrModel.applicationRegion,
                    odrModel.toImageCoordinates);

}


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

struct ExtendedRegion
{
private:
    static int _width(const DigitalSet& ds)
    {
        DGtal::Z2i::Point lb,ub;
        ds.computeBoundingBox(lb,ub);
        DGtal::Z2i::Point dimSize = ub-lb+DGtal::Z2i::Point(1,1);

        return dimSize(0);
    }

    static int _height(const DigitalSet& ds)
    {
        DGtal::Z2i::Point lb,ub;
        ds.computeBoundingBox(lb,ub);
        DGtal::Z2i::Point dimSize = ub-lb+DGtal::Z2i::Point(1,1);

        return dimSize(1);
    }
public:
    ExtendedRegion(const DigitalSet& ds,
                   int border):digitalSet(ds),
                               border(border),
                               width(_width(ds)),
                               height(_height(ds)),
                               extW(width+2*border),
                               extH(height+2*border)
    {}

    const DigitalSet& digitalSet;
    const int border;
    const int width;
    const int height;
    const int extW;
    const int extH;
};

CellType resolveCellType(const ExtendedRegion& extRegion,int row,int col)
{
    int border = extRegion.border;
    row -= border;
    col -= border;

    if(row<=-(border+1) || col<=-(border+1)) return CellType::Not_Defined;
    if(row>=(extRegion.extH-border) || col>=(extRegion.extW-border)) return CellType::Not_Defined;

    if(row<=-border || col<=-border) return CellType::Border;
    if(row>=(extRegion.extH-border-1) || col>=(extRegion.extW-border-1)) return CellType::Border;

    if(row<=-1 || col<=-1) return CellType::Aux;
    if(row>=extRegion.height || col>=extRegion.width) return CellType::Aux;

    return CellType::Normal;
}

void createPixelSet(std::vector<Pixel>& pxlVector,
                    const ExtendedRegion& extRegion)
{
    int h = extRegion.extH;
    int w = extRegion.extW;

    int varIndex=0;
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            CellType ct = resolveCellType(extRegion,i,j);
            if(ct==CellType::Not_Defined) continue;

            pxlVector.push_back(Pixel(i,j,ct,varIndex++));
        }
    }
}

int main(int argc, char* argv[])
{
    DigitalSet ds = square();
    ODRModel odrModel = odrModel(ds);



    return 0;
}
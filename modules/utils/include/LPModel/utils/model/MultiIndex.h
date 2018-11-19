#ifndef LPMODEL_MULTIINDEX_H
#define LPMODEL_MULTIINDEX_H

#include <vector>
#include <assert.h>

namespace LPModel
{
    namespace Utils
    {
        template<typename TIndex>
        class MultiIndex
        {
        public:
            typedef TIndex Index;
            typedef MultiIndex<Index> Self;

            typedef std::vector<Index> Container;
            typedef typename Container::const_iterator ConstIterator;

            Self& operator<<(const Index& element)
            {
                this->elements.push_back(element);
                std::sort( this->elements.begin(),this->elements.end() );

                return *this;
            }

            bool operator<(const Self& other) const
            {
                assert(this->elements.size()==other.elements.size());

                ConstIterator it1 = this->elements.begin();
                ConstIterator it2 = other.elements.begin();

                while(*it1==*it2)
                {
                    ++it1;
                    ++it2;
                }

                return *it1 < *it2;
            }

            ConstIterator begin() const{return elements.begin();}
            ConstIterator end() const{return elements.end();}

        private:
            Container elements;

        };


    }
}

#endif //LPMODEL_MULTIINDEX_H

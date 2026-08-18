#ifndef ORDINAL_H
#define ORDINAL_H

class Ordinal {
private:
    int omegaCount;
    int finiteSize;

public:
    Ordinal(int omegaCount, int finiteSize): omegaCount(omegaCount), finiteSize(finiteSize) {}

    bool IsInfinite() const {
        return omegaCount != 0;
    }

    int GetOmegaCount() const {
        return omegaCount;
    }

    int GetFiniteSize() const {
        return finiteSize;
    }
    
    Ordinal operator+(const Ordinal& other) const {
        int newOmegaCount, newFiniteSize;
        if (other.IsInfinite()) {
            newOmegaCount = omegaCount + other.omegaCount;
            newFiniteSize = other.finiteSize;
        }
        else {
            newOmegaCount = omegaCount;
            newFiniteSize = finiteSize + other.finiteSize;
        }
        return Ordinal(newOmegaCount, newFiniteSize);
    }

    bool operator==(const Ordinal& other) const {
        return omegaCount == other.omegaCount && finiteSize == other.finiteSize;
    }

    bool operator<(const Ordinal& other) const {
        if (omegaCount < other.omegaCount)
            return true;
        if (omegaCount > other.omegaCount)
            return false;
        else {
            return finiteSize < other.finiteSize;
        }
    }   

    bool operator<=(const Ordinal& other) const {
        if (omegaCount < other.omegaCount)
            return true;
        if (omegaCount == other.omegaCount && finiteSize <= other.finiteSize)
            return true;
        if (omegaCount == other.omegaCount && finiteSize > other.finiteSize)
            return false;   
        else
            return false;
    }  
    
};

#endif
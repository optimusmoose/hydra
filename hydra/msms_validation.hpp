#ifndef msms_validation_hpp
#define msms_validation_hpp

#include <stdio.h>
#include <vector>
#include <memory>

#include "cluster.hpp"

class MSMSValidation
{
    std::vector<std::vector<std::unique_ptr<Cluster>>>& clusters;
public:
    MSMSValidation(std::vector<std::vector<std::unique_ptr<Cluster>>>& clusters_);
    void perform();
private:
    bool valid_cluster(Cluster* cluster);
    // Add the delete clusters if they're soft-deleted here, use inheritance
};

#endif /* msms_validation_hpp */

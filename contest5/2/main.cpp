#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;

namespace market
{
    struct Offer
    {
        std::string offer_id;
        int market_sku;
        int price;

        bool operator< (Offer const& rhs) const
        {
            return std::tie(this->price, this->offer_id) < std::tie(rhs.price, rhs.offer_id);
        }
    };

    void to_json(json& j, Offer const& offer) 
    {
        j = json
        {
            {"offer_id", offer.offer_id},
            {"market_sku", offer.market_sku},
            {"price", offer.price}
        };
    }

    void from_json(json const& j, Offer& offer) 
    {
        j.at("offer_id").get_to(offer.offer_id);
        j.at("market_sku").get_to(offer.market_sku);
        j.at("price").get_to(offer.price);
    }
}


using OfferIter = std::vector<market::Offer>::const_iterator;
using OfferVector = std::vector<market::Offer>;

struct QueueEntry
{
    size_t feedIndex;
    OfferIter offerIt;

    bool operator> (QueueEntry const& rhs) const
    {
        return *(rhs.offerIt) < *(this->offerIt);
    }
};

OfferVector MergeKSortedFeeds(std::vector<OfferVector> const& feeds)
{
    std::priority_queue<
        QueueEntry, 
        std::vector<QueueEntry>, 
        std::greater<QueueEntry>
    > queue;

    std::vector<OfferIter> nextOffer(feeds.size());

    for(size_t i = 0; i < feeds.size(); ++i)
    {
        queue.push( QueueEntry{ i, feeds[i].begin() } );
        nextOffer[i] = std::next(feeds[i].begin());
    }

    OfferVector res;

    while(!queue.empty())
    {
        QueueEntry top = queue.top();
        res.push_back(*top.offerIt);
        queue.pop();

        OfferIter& next = nextOffer[top.feedIndex];

        if(next != feeds[top.feedIndex].end())
        {
            queue.push( QueueEntry{ top.feedIndex, next } );
            ++next;
        }
    }

    return res;
}

int main() 
{
    std::ifstream input("input.txt");

    int feedCount;
    input >> feedCount;
    
    std::vector<OfferVector> feeds;

    for(size_t i = 0; i < feedCount; ++i) 
    {
        json jsonObj;
        input >> jsonObj;

        if(jsonObj.contains("offers"))
        {
            auto offers = jsonObj["offers"].get<OfferVector>();
            std::sort(offers.begin(), offers.end());
            feeds.push_back(std::move(offers));
        }
    }

    OfferVector mergedOffers = MergeKSortedFeeds(feeds);

    json res;
    res["offers"] = std::move(mergedOffers);

    std::ofstream output("output.txt");
    output << res;

    return 0;
}
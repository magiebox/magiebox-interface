#include <eosiolib/asset.hpp>
#include "boxcomponent/boxplugin.hpp"

class[[eosio::contract("demo")]] demo : public eosio::contract
{
public:
    demo(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}
    void transfer(name from, name to, asset quantity, string memo);
};

EOSIO_DISPATCH_BOX(demo, (transfer))

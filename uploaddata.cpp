#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
using namespace eosio;
using std::string;

class uploaddata : public eosio::contract {
private:
    /// @abi table profiles
    struct profile {
        uint64_t       key;
        string       value;

        uint64_t primary_key() const { return key; }

        EOSLIB_SERIALIZE(profile, (key)(value))
    };
    


public:
    using contract::contract;

    explicit uploaddata(account_name self) : contract(self) {} 

    /// @abi action
    void upload(const account_name account,
		    const uint64_t key,
		    const string& value) 
    {
        require_auth(_self);
        
	profile_table_t profile(_self,_self);

	auto iter = profile.find(key);
	eosio_assert(iter == profile.end(),"Account already has profile.");

	profile.emplace(account, [&](struct profile& p){
	    p.key = key;
	    p.value = value;
	});

        require_recipient(_self);
	print("profile upload");
    }

    /// @abi action
    void update(const account_name account,
		    const uint64_t key,
                    const string& value)
    {
        require_auth(_self);
        //require_auth(N(ecdowsurecom));
        profile_table_t profile(_self,_self);
        auto iter = profile.find(key);

        profile.modify(iter, account, [&](struct profile& p){
            p.key = key;
            p.value = value;
        });
        require_recipient(_self);
        print("profile modify");

    }

    /// @abi action
    void show(uint64_t key) 
    {
        profile_table_t profile(_self,_self);

        auto iter = profile.find(key);
        eosio_assert(iter != profile.end(),"Account dose not have a profile.");
	print(iter->value.c_str());
    }

    /// @abi action
    void remove(const account_name account,
		    const uint64_t key)
    {
        require_auth(_self);
        //require_auth(N(ecdowsurecom));
        profile_table_t profile(_self,_self);

        auto iter = profile.find(key);
        eosio_assert(iter != profile.end(),"Account dose not have a profile.");
    
        profile.erase(iter);
	require_recipient(_self);
	print("profile removed");
    }


private:

    using profile_table_t = eosio::multi_index<N(profiles),profile>;
};
EOSIO_ABI(uploaddata, (upload)(update)(show)(remove))

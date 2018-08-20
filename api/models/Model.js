// const bloom = require("bloom-filter");
var BloomFilter = require('../../build/Release/bloom-dash');
var Base58 = require('base58');
var listBloom = [];
var listTrunks  = [];


const  CreateListBloom = function(number) {

    for (var i = 0; i < number; i++) {
        var bloom =BloomFilter.Bloom(
            listTrunks[i].length,    // listTrunks[i].length: nb of elements
            0.05,
            0,       //ntweak
            0
        );
        listBloom.push(bloom);

    }
    console.log("bloom added " + listBloom.length);
    return listBloom;
}

const VerifyBlooms = function(addr){
    for (let bloomIndex in listBloom) {
        let bloom = listBloom[bloomIndex];
        if(!bloom.contains(addr))
            {
                //console.log("No." + bloomIndex +" doesn't have the address: " + addr);
            }else{
                 //console.log("No." + bloomIndex +" has the address: " + addr);
            }
    }
}

const MakeTrunks = function(num){

    for(let i=0;i<num;i++){
        var trunk = [];
        trunk.length = Math.floor(Math.random()*400) + 100;
        // console.log("trunk "+ i +"has length: " + trunk.length);
        listTrunks.push(trunk);
    }
    console.log("trunks done!");
    return listTrunks;

}

const Base58Decode = function (addr) {

    return Base58.decode(addr);
}

const LoadTrunks = function(i,j,addr){
    addr = addr.substr(1,addr.length);        //delete the first char X
    console.log(i+" "+j+" "+addr);

    listTrunks[i][j] = Base58Decode(addr);
    listBloom[i].insert(Base58Decode(addr));
}

module.exports.CreateListBloom = CreateListBloom;
module.exports.VerifyBlooms = VerifyBlooms;
module.exports.MakeTrunks = MakeTrunks;
module.exports.LoadTrunks = LoadTrunks;
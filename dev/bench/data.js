window.BENCHMARK_DATA = {
  "lastUpdate": 1778821683155,
  "repoUrl": "https://github.com/mblackman/Octarine-Engine",
  "entries": {
    "Octarine Engine Micro-Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "09244773702906313f383b89db5784b2fb322501",
          "message": "Update benchmark.yml",
          "timestamp": "2026-05-11T21:10:26-06:00",
          "tree_id": "54536db3ceb47b8cb58050f7f06289d29e46b930",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/09244773702906313f383b89db5784b2fb322501"
        },
        "date": 1778556609883,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3456.929890613707,
            "unit": "ns/iter",
            "extra": "iterations: 194968\ncpu: 3494.0109864185015 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 13648.756096175424,
            "unit": "ns/iter",
            "extra": "iterations: 50519\ncpu: 13692.249807002885 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 88290.2636313799,
            "unit": "ns/iter",
            "extra": "iterations: 7770\ncpu: 88335.26872587255 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 764075.3538860058,
            "unit": "ns/iter",
            "extra": "iterations: 929\ncpu: 764050.5382131365 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1553313.0319245094,
            "unit": "ns/iter",
            "extra": "iterations: 452\ncpu: 1553134.6349557445 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 1911.1642883828724,
            "unit": "ns/iter",
            "extra": "iterations: 372430\ncpu: 1884.8357248337854 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 4524.476659833194,
            "unit": "ns/iter",
            "extra": "iterations: 155857\ncpu: 4487.666861286538 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 25002.183391647046,
            "unit": "ns/iter",
            "extra": "iterations: 28027\ncpu: 24966.756912975674 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 190207.6695747927,
            "unit": "ns/iter",
            "extra": "iterations: 3675\ncpu: 190117.90530610576 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 402489.20603002654,
            "unit": "ns/iter",
            "extra": "iterations: 1731\ncpu: 402371.6545349249 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2676a5363e781349b5cec54a669c5e415a6c4763",
          "message": "Use a struct tag for projectiles",
          "timestamp": "2026-05-11T21:44:18-06:00",
          "tree_id": "5f11b79232c76aa84620f22d1219a7fb001d78c7",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2676a5363e781349b5cec54a669c5e415a6c4763"
        },
        "date": 1778558225934,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3818.09826108749,
            "unit": "ns/iter",
            "extra": "iterations: 180004\ncpu: 3857.914607453156 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15176.437546729761,
            "unit": "ns/iter",
            "extra": "iterations: 46710\ncpu: 15221.147377434287 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 101060.92939864348,
            "unit": "ns/iter",
            "extra": "iterations: 6887\ncpu: 101114.27646290151 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 859129.504003038,
            "unit": "ns/iter",
            "extra": "iterations: 823\ncpu: 858534.5540704767 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1734782.5906730343,
            "unit": "ns/iter",
            "extra": "iterations: 406\ncpu: 1734820.3497537198 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2232.837812036788,
            "unit": "ns/iter",
            "extra": "iterations: 318775\ncpu: 2202.2769069092205 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6491.464748230239,
            "unit": "ns/iter",
            "extra": "iterations: 108019\ncpu: 6456.151538157035 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40568.84696109123,
            "unit": "ns/iter",
            "extra": "iterations: 17371\ncpu: 40525.164066543446 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 311537.2657991693,
            "unit": "ns/iter",
            "extra": "iterations: 2244\ncpu: 311435.0762031487 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 645160.585531686,
            "unit": "ns/iter",
            "extra": "iterations: 1082\ncpu: 645001.7892791599 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "152f8e2e2337bbb79e8c74918afc9ec7b8490f6e",
          "message": "Fix projectiles masks spawned by the projectile emitter",
          "timestamp": "2026-05-11T22:25:47-06:00",
          "tree_id": "0edbd2d84978d058fd3bbc0e2ad5375cd6472018",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/152f8e2e2337bbb79e8c74918afc9ec7b8490f6e"
        },
        "date": 1778560363752,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3799.423877590467,
            "unit": "ns/iter",
            "extra": "iterations: 183066\ncpu: 3835.772595675822 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15245.192747241284,
            "unit": "ns/iter",
            "extra": "iterations: 45895\ncpu: 15288.657784070521 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102037.4697969949,
            "unit": "ns/iter",
            "extra": "iterations: 6862\ncpu: 102095.13538327305 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 853094.9696633022,
            "unit": "ns/iter",
            "extra": "iterations: 814\ncpu: 853087.41769042 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1737249.292960164,
            "unit": "ns/iter",
            "extra": "iterations: 403\ncpu: 1737349.4764268147 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2216.0882155504055,
            "unit": "ns/iter",
            "extra": "iterations: 330849\ncpu: 2173.9339154722006 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6492.077459859677,
            "unit": "ns/iter",
            "extra": "iterations: 108790\ncpu: 6439.404035293947 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40568.23403754909,
            "unit": "ns/iter",
            "extra": "iterations: 17291\ncpu: 40519.091550507444 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 313031.017949331,
            "unit": "ns/iter",
            "extra": "iterations: 2226\ncpu: 312924.5597484005 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653344.8041614469,
            "unit": "ns/iter",
            "extra": "iterations: 1069\ncpu: 653183.9569691714 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "ef5d49329d8b293539cb6c4a168adece6b2a0574",
          "message": "Update benchmark for what should be more consistent results",
          "timestamp": "2026-05-11T22:38:58-06:00",
          "tree_id": "14dff3d9af8b2516842bcf434b9bb7c07d377a5b",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/ef5d49329d8b293539cb6c4a168adece6b2a0574"
        },
        "date": 1778561143934,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3834.6458014707187,
            "unit": "ns/iter",
            "extra": "iterations: 178457\ncpu: 3869.841110183466 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15535.275013756045,
            "unit": "ns/iter",
            "extra": "iterations: 44644\ncpu: 15576.17906101668 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102246.82427595508,
            "unit": "ns/iter",
            "extra": "iterations: 6866\ncpu: 102281.77803670181 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 857785.212138862,
            "unit": "ns/iter",
            "extra": "iterations: 815\ncpu: 857857.3509202487 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1743814.4446234799,
            "unit": "ns/iter",
            "extra": "iterations: 402\ncpu: 1743966.0895522377 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2285.2716751964404,
            "unit": "ns/iter",
            "extra": "iterations: 311949\ncpu: 2249.7154086076775 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6728.110987665132,
            "unit": "ns/iter",
            "extra": "iterations: 105068\ncpu: 6688.48693227416 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42291.23489716864,
            "unit": "ns/iter",
            "extra": "iterations: 16553\ncpu: 42253.935359154275 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 327032.3030145015,
            "unit": "ns/iter",
            "extra": "iterations: 2122\ncpu: 326930.978322309 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 678086.4162551977,
            "unit": "ns/iter",
            "extra": "iterations: 1037\ncpu: 677934.2748313035 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "de5709d23833ca5d9c6cbad913de7525f07e84b2",
          "message": "Render walking optimizations\n\nSingle-pass histogram — Build all 8 byte-histograms in one scan instead of 8 separate scans, eliminating ~3.5MB of redundant reads per sort\nSkip-uniform-byte passes — Detect bytes that are identical across all entries (e.g., shared layer) and skip those radix passes entirely. Cuts 8 passes to ~4 in the stress scene\nGather-based permutation — Replace the cycle-walk (random pointer chasing through 80-byte objects) with a sequential-write gather into a scratch buffer",
          "timestamp": "2026-05-11T22:59:01-06:00",
          "tree_id": "916e67143d9fcf476eb9317af1c9a56c4bdc2ea3",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/de5709d23833ca5d9c6cbad913de7525f07e84b2"
        },
        "date": 1778562354628,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3788.0753768039935,
            "unit": "ns/iter",
            "extra": "iterations: 183300\ncpu: 3827.5823349699567 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15348.905067644971,
            "unit": "ns/iter",
            "extra": "iterations: 45568\ncpu: 15393.833699087792 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102655.79103891738,
            "unit": "ns/iter",
            "extra": "iterations: 6669\ncpu: 102719.63547757993 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 859713.6838588773,
            "unit": "ns/iter",
            "extra": "iterations: 820\ncpu: 859814.3341463477 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1740621.3041534298,
            "unit": "ns/iter",
            "extra": "iterations: 401\ncpu: 1740683.4488778133 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2250.3189991363733,
            "unit": "ns/iter",
            "extra": "iterations: 316921\ncpu: 2208.437998744909 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6475.797324603175,
            "unit": "ns/iter",
            "extra": "iterations: 108091\ncpu: 6431.413919751903 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40906.116104617526,
            "unit": "ns/iter",
            "extra": "iterations: 17049\ncpu: 40843.14692944409 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316219.2182000618,
            "unit": "ns/iter",
            "extra": "iterations: 2239\ncpu: 316080.52612775756 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 655022.1498813273,
            "unit": "ns/iter",
            "extra": "iterations: 1076\ncpu: 654858.7908921909 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2a557375ca25b71ebc508e7a1e694b1a64bf2631",
          "message": "Optimize Registry::Update and system instrumentation\n\nCameraFollowSystem — Scoped to CameraFollowComponent to avoid O(N) execution (fixed ~4.7ms bottleneck)\n\nGather Boxes — Parallelized collision data gathering using ParallelForEach\n\nQuery Counting — Added GetCount() to ComponentQuery for pre-allocating result vectors\n\nType Prettifying — Improved PrettifyTypeName to trim GCC/Clang leading length prefixes",
          "timestamp": "2026-05-11T23:18:23-06:00",
          "tree_id": "f9e6e49d9442b2255075ee70d57f80d7da9bd485",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2a557375ca25b71ebc508e7a1e694b1a64bf2631"
        },
        "date": 1778563513803,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3794.658491014893,
            "unit": "ns/iter",
            "extra": "iterations: 181811\ncpu: 3826.0031791257106 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15147.188883997018,
            "unit": "ns/iter",
            "extra": "iterations: 45159\ncpu: 15187.851170309064 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 101024.00431233429,
            "unit": "ns/iter",
            "extra": "iterations: 6856\ncpu: 101069.0689906671 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 855859.8107915636,
            "unit": "ns/iter",
            "extra": "iterations: 818\ncpu: 855840.7665036592 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1731923.6884353464,
            "unit": "ns/iter",
            "extra": "iterations: 401\ncpu: 1731987.2443889813 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2247.4643449060436,
            "unit": "ns/iter",
            "extra": "iterations: 317489\ncpu: 2214.678335313486 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6496.071067302138,
            "unit": "ns/iter",
            "extra": "iterations: 108051\ncpu: 6459.897816769269 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40860.93843188749,
            "unit": "ns/iter",
            "extra": "iterations: 17040\ncpu: 40828.28926056052 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 313982.13541545125,
            "unit": "ns/iter",
            "extra": "iterations: 2231\ncpu: 313895.8946660788 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653873.5875101001,
            "unit": "ns/iter",
            "extra": "iterations: 1080\ncpu: 653707.862037124 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "ae8731c72154e0a0d311a1fbaedae6a0d6ace2d9",
          "message": "Added GLM SIMD compiler flags",
          "timestamp": "2026-05-12T08:45:03-06:00",
          "tree_id": "2aae62c6a78338b500f18c60ab82709f3925b29e",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/ae8731c72154e0a0d311a1fbaedae6a0d6ace2d9"
        },
        "date": 1778597569247,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3852.442282607348,
            "unit": "ns/iter",
            "extra": "iterations: 183112\ncpu: 3887.682336493865 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15057.258826137306,
            "unit": "ns/iter",
            "extra": "iterations: 46059\ncpu: 15096.44034824803 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 101420.86743059468,
            "unit": "ns/iter",
            "extra": "iterations: 6860\ncpu: 101471.10379008953 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 842340.6679414095,
            "unit": "ns/iter",
            "extra": "iterations: 823\ncpu: 842446.0607533404 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1734995.2982077675,
            "unit": "ns/iter",
            "extra": "iterations: 403\ncpu: 1734911.0173697304 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2135.0512517489665,
            "unit": "ns/iter",
            "extra": "iterations: 334029\ncpu: 2100.356478029445 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6691.892745844494,
            "unit": "ns/iter",
            "extra": "iterations: 106560\ncpu: 6656.813532278526 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41869.751087923265,
            "unit": "ns/iter",
            "extra": "iterations: 16738\ncpu: 41834.920002392886 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 323888.52588269877,
            "unit": "ns/iter",
            "extra": "iterations: 2159\ncpu: 323799.1042148985 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 668534.6138388832,
            "unit": "ns/iter",
            "extra": "iterations: 1044\ncpu: 668359.1867815871 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "261abb78b3fc52496bf6c5476c918419e150a2e2",
          "message": "Improvements to debug UI.\n\nEnabled docking for igui\nAdded save state for editor layout.\nImproved entity browser component.",
          "timestamp": "2026-05-12T22:12:44-06:00",
          "tree_id": "72bfa64fb67db86e1e4381d0a2f4f393836457f1",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/261abb78b3fc52496bf6c5476c918419e150a2e2"
        },
        "date": 1778646077269,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3801.958486785192,
            "unit": "ns/iter",
            "extra": "iterations: 182300\ncpu: 3836.820213932945 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15366.16942504811,
            "unit": "ns/iter",
            "extra": "iterations: 45812\ncpu: 15405.725552257094 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102826.22527227068,
            "unit": "ns/iter",
            "extra": "iterations: 6767\ncpu: 102885.92138318473 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 856275.2800050955,
            "unit": "ns/iter",
            "extra": "iterations: 821\ncpu: 856351.6467722261 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1736285.0368954241,
            "unit": "ns/iter",
            "extra": "iterations: 400\ncpu: 1736304.8675000048 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2281.8472096155797,
            "unit": "ns/iter",
            "extra": "iterations: 313055\ncpu: 2248.7690980815314 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6831.6847760456985,
            "unit": "ns/iter",
            "extra": "iterations: 102993\ncpu: 6795.005039175121 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42837.33380529578,
            "unit": "ns/iter",
            "extra": "iterations: 16361\ncpu: 42784.67948170059 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 331537.29055332235,
            "unit": "ns/iter",
            "extra": "iterations: 2115\ncpu: 331425.64680850005 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 681807.6564017141,
            "unit": "ns/iter",
            "extra": "iterations: 1030\ncpu: 681697.7368932103 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "6402bedc3d06a118a0ee7dcdb5856ca0b87990df",
          "message": "Improved look of editor windows to be easier to see on different display types",
          "timestamp": "2026-05-12T22:19:46-06:00",
          "tree_id": "de6342f33380ff67c8ce97ea2a5c4973a1ec0386",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/6402bedc3d06a118a0ee7dcdb5856ca0b87990df"
        },
        "date": 1778646738288,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3792.794752251766,
            "unit": "ns/iter",
            "extra": "iterations: 183880\ncpu: 3826.9677126388137 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15715.257967709314,
            "unit": "ns/iter",
            "extra": "iterations: 46426\ncpu: 15754.11364321679 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 101836.2810562249,
            "unit": "ns/iter",
            "extra": "iterations: 6632\ncpu: 101896.05925814576 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 862152.9906298444,
            "unit": "ns/iter",
            "extra": "iterations: 821\ncpu: 862237.1705237586 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1729874.9089241028,
            "unit": "ns/iter",
            "extra": "iterations: 400\ncpu: 1730018.979999981 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2233.999940984683,
            "unit": "ns/iter",
            "extra": "iterations: 318025\ncpu: 2200.2284631704847 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6462.54979014102,
            "unit": "ns/iter",
            "extra": "iterations: 108837\ncpu: 6418.675643392658 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40084.32897000389,
            "unit": "ns/iter",
            "extra": "iterations: 17408\ncpu: 40032.3374885114 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 311656.79078919865,
            "unit": "ns/iter",
            "extra": "iterations: 2261\ncpu: 311535.4847413033 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 658554.7445736735,
            "unit": "ns/iter",
            "extra": "iterations: 1071\ncpu: 658341.5854341108 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "a221607cd1f8c572a2094ecaf5b5bd774b95620d",
          "message": "Maintain game aspect ratio in debug editor mode",
          "timestamp": "2026-05-12T22:24:40-06:00",
          "tree_id": "5a363c70ff3cd98a6ba98be1b86c71d9352370cd",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a221607cd1f8c572a2094ecaf5b5bd774b95620d"
        },
        "date": 1778647559655,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3791.421343389629,
            "unit": "ns/iter",
            "extra": "iterations: 183275\ncpu: 3831.9794134497215 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 14887.039621673368,
            "unit": "ns/iter",
            "extra": "iterations: 46964\ncpu: 14933.930137978483 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 103132.02250360172,
            "unit": "ns/iter",
            "extra": "iterations: 6565\ncpu: 103184.14562071292 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 868474.6193466708,
            "unit": "ns/iter",
            "extra": "iterations: 800\ncpu: 868490.0887499952 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1731233.288635417,
            "unit": "ns/iter",
            "extra": "iterations: 404\ncpu: 1731345.5346534548 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2229.2920146059123,
            "unit": "ns/iter",
            "extra": "iterations: 318793\ncpu: 2192.178789998982 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6465.097627690633,
            "unit": "ns/iter",
            "extra": "iterations: 108727\ncpu: 6423.799083943857 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40392.22932024438,
            "unit": "ns/iter",
            "extra": "iterations: 17398\ncpu: 40350.57293940948 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 311999.41517956334,
            "unit": "ns/iter",
            "extra": "iterations: 2242\ncpu: 311906.6297948197 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 652188.9554888176,
            "unit": "ns/iter",
            "extra": "iterations: 1074\ncpu: 652015.02234634 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "482def5e5a6a6c505030170a51769df61b55a5e4",
          "message": "Add Lua console window and history management",
          "timestamp": "2026-05-12T22:33:36-06:00",
          "tree_id": "17ee815489bcd3680406d0ca5481f5ae397dc62a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/482def5e5a6a6c505030170a51769df61b55a5e4"
        },
        "date": 1778648872638,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3751.7680806683866,
            "unit": "ns/iter",
            "extra": "iterations: 184934\ncpu: 3781.4287529608214 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 14890.192536627646,
            "unit": "ns/iter",
            "extra": "iterations: 47835\ncpu: 14929.551060940266 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 99045.7658139971,
            "unit": "ns/iter",
            "extra": "iterations: 6942\ncpu: 99093.95764909161 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 835356.8421263328,
            "unit": "ns/iter",
            "extra": "iterations: 838\ncpu: 835417.5226730204 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1708379.1678507926,
            "unit": "ns/iter",
            "extra": "iterations: 403\ncpu: 1708364.3647642587 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2227.8770816414703,
            "unit": "ns/iter",
            "extra": "iterations: 318881\ncpu: 2197.1298070440644 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6547.395290586238,
            "unit": "ns/iter",
            "extra": "iterations: 108356\ncpu: 6511.151269886039 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41102.6400097509,
            "unit": "ns/iter",
            "extra": "iterations: 17104\ncpu: 41060.02525724625 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 314894.2784999739,
            "unit": "ns/iter",
            "extra": "iterations: 2233\ncpu: 314780.41244960256 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 658811.5629466561,
            "unit": "ns/iter",
            "extra": "iterations: 1042\ncpu: 658603.5729366428 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "b90c4b5c84dd94d251b562109833d11138f9ad9d",
          "message": "Fix Windows build",
          "timestamp": "2026-05-12T22:37:43-06:00",
          "tree_id": "b3426092d2016fe5c72ae15d4f740a73f5e9a45e",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/b90c4b5c84dd94d251b562109833d11138f9ad9d"
        },
        "date": 1778650448166,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3781.128490083891,
            "unit": "ns/iter",
            "extra": "iterations: 183842\ncpu: 3814.707944865804 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15066.859692234424,
            "unit": "ns/iter",
            "extra": "iterations: 45870\ncpu: 15108.702071069652 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 100917.58731527062,
            "unit": "ns/iter",
            "extra": "iterations: 6672\ncpu: 100964.89598321184 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 847978.464983963,
            "unit": "ns/iter",
            "extra": "iterations: 826\ncpu: 848114.4152542236 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1719479.0969800039,
            "unit": "ns/iter",
            "extra": "iterations: 406\ncpu: 1719479.763546786 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2239.72984423344,
            "unit": "ns/iter",
            "extra": "iterations: 315785\ncpu: 2205.567351202784 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6579.309499129592,
            "unit": "ns/iter",
            "extra": "iterations: 105996\ncpu: 6540.928761467265 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40805.473023568935,
            "unit": "ns/iter",
            "extra": "iterations: 17010\ncpu: 40765.87507347896 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 317297.5097201828,
            "unit": "ns/iter",
            "extra": "iterations: 2223\ncpu: 317159.4579397364 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 656000.0560516231,
            "unit": "ns/iter",
            "extra": "iterations: 1070\ncpu: 655847.1158878157 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "9d0cc87a140fe800b7ded86e5dd807d779fb7a5b",
          "message": "Fix build workflows",
          "timestamp": "2026-05-14T21:36:57-06:00",
          "tree_id": "415c180937f64d692c27b622b546d0728d30e2c2",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/9d0cc87a140fe800b7ded86e5dd807d779fb7a5b"
        },
        "date": 1778816643001,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3998.624081361417,
            "unit": "ns/iter",
            "extra": "iterations: 174262\ncpu: 4034.97200192789 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16208.574607418963,
            "unit": "ns/iter",
            "extra": "iterations: 43236\ncpu: 16257.962947543898 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 108054.42218529247,
            "unit": "ns/iter",
            "extra": "iterations: 6502\ncpu: 108110.13134420261 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 900672.890541964,
            "unit": "ns/iter",
            "extra": "iterations: 755\ncpu: 900747.1907284813 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1817959.7645939793,
            "unit": "ns/iter",
            "extra": "iterations: 384\ncpu: 1817530.5729167096 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2284.412729845404,
            "unit": "ns/iter",
            "extra": "iterations: 311213\ncpu: 2274.6953147836284 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10143.200754390109,
            "unit": "ns/iter",
            "extra": "iterations: 69273\ncpu: 10133.9998123349 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 103055.21093068867,
            "unit": "ns/iter",
            "extra": "iterations: 6797\ncpu: 103031.18875976616 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 395195.1376473623,
            "unit": "ns/iter",
            "extra": "iterations: 1742\ncpu: 395148.1084959826 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2173.9049296506782,
            "unit": "ns/iter",
            "extra": "iterations: 325995\ncpu: 2142.6532860910243 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6663.131231341706,
            "unit": "ns/iter",
            "extra": "iterations: 105612\ncpu: 6629.43048138156 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42015.51987260995,
            "unit": "ns/iter",
            "extra": "iterations: 16668\ncpu: 41984.00227980757 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 327730.82277978235,
            "unit": "ns/iter",
            "extra": "iterations: 2136\ncpu: 327587.9831460699 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 665517.9093513003,
            "unit": "ns/iter",
            "extra": "iterations: 1051\ncpu: 665440.4310181367 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "6775a29273dc1cd00263b1f8f7c23bc644179810",
          "message": "Support editor and player builds. Macos support",
          "timestamp": "2026-05-14T21:48:38-06:00",
          "tree_id": "b66c6cec2b5b45bc5e85815107628a5309d27c9b",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/6775a29273dc1cd00263b1f8f7c23bc644179810"
        },
        "date": 1778817327675,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4002.504796314777,
            "unit": "ns/iter",
            "extra": "iterations: 168861\ncpu: 4036.6644636714354 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16219.260860462677,
            "unit": "ns/iter",
            "extra": "iterations: 43183\ncpu: 16253.065141373803 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107740.32057908647,
            "unit": "ns/iter",
            "extra": "iterations: 6482\ncpu: 107791.49784017715 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 911731.1617780116,
            "unit": "ns/iter",
            "extra": "iterations: 775\ncpu: 911759.952258067 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1828349.4728619815,
            "unit": "ns/iter",
            "extra": "iterations: 381\ncpu: 1828388.1102362263 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2407.6384000668118,
            "unit": "ns/iter",
            "extra": "iterations: 289749\ncpu: 2404.535139033885 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10259.149806371364,
            "unit": "ns/iter",
            "extra": "iterations: 67162\ncpu: 10256.15391144187 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 70191.62174125647,
            "unit": "ns/iter",
            "extra": "iterations: 9994\ncpu: 70167.09635781596 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 393185.9474895256,
            "unit": "ns/iter",
            "extra": "iterations: 1764\ncpu: 393165.67970524123 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2186.313988233034,
            "unit": "ns/iter",
            "extra": "iterations: 325541\ncpu: 2150.8379896857737 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6801.172671228486,
            "unit": "ns/iter",
            "extra": "iterations: 103174\ncpu: 6766.367844613264 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 43152.28226502307,
            "unit": "ns/iter",
            "extra": "iterations: 16282\ncpu: 43104.59587275266 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 324189.3860595436,
            "unit": "ns/iter",
            "extra": "iterations: 2141\ncpu: 324077.7505838346 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 679734.2254364804,
            "unit": "ns/iter",
            "extra": "iterations: 1029\ncpu: 679519.9475218571 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "b1c29c587258376dcae372bfc33d78db049520f0",
          "message": "Simplify parameter args and update readme",
          "timestamp": "2026-05-14T22:01:28-06:00",
          "tree_id": "121ffa1d7bb5a717963ff917f43182d690f3efca",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/b1c29c587258376dcae372bfc33d78db049520f0"
        },
        "date": 1778818108050,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4080.5492286759923,
            "unit": "ns/iter",
            "extra": "iterations: 172732\ncpu: 4120.198116156882 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15907.878994642815,
            "unit": "ns/iter",
            "extra": "iterations: 42974\ncpu: 15955.541560012929 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 109056.90747566673,
            "unit": "ns/iter",
            "extra": "iterations: 6430\ncpu: 109119.85194400977 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 910412.8745565941,
            "unit": "ns/iter",
            "extra": "iterations: 770\ncpu: 910455.8064935086 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1851297.6197748191,
            "unit": "ns/iter",
            "extra": "iterations: 377\ncpu: 1851376.8063660804 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2375.255993368068,
            "unit": "ns/iter",
            "extra": "iterations: 297348\ncpu: 2364.817358785984 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10097.687057237534,
            "unit": "ns/iter",
            "extra": "iterations: 69918\ncpu: 10087.5174919215 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 101936.20822160492,
            "unit": "ns/iter",
            "extra": "iterations: 6847\ncpu: 101930.49087191265 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 402750.55460657453,
            "unit": "ns/iter",
            "extra": "iterations: 1746\ncpu: 402699.0435280491 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2258.828605710038,
            "unit": "ns/iter",
            "extra": "iterations: 315453\ncpu: 2231.1549739598267 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6710.764128301442,
            "unit": "ns/iter",
            "extra": "iterations: 102911\ncpu: 6678.387888572413 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42613.61708677296,
            "unit": "ns/iter",
            "extra": "iterations: 16610\ncpu: 42575.61637566261 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 327229.5061066837,
            "unit": "ns/iter",
            "extra": "iterations: 2147\ncpu: 327125.05030276335 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 652203.0434701766,
            "unit": "ns/iter",
            "extra": "iterations: 1070\ncpu: 652121.1065420624 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "632262380642f78c0b7d1e30c22916c967d31faf",
          "message": "Hopefully fix macos build",
          "timestamp": "2026-05-14T22:21:21-06:00",
          "tree_id": "68d9e72dd57a19b15620ad9d16ed7ad23aea5b5a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/632262380642f78c0b7d1e30c22916c967d31faf"
        },
        "date": 1778819298404,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4058.8751958910693,
            "unit": "ns/iter",
            "extra": "iterations: 172594\ncpu: 4093.787176842308 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16835.466728101866,
            "unit": "ns/iter",
            "extra": "iterations: 41115\ncpu: 16877.171956706316 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 109061.67743677222,
            "unit": "ns/iter",
            "extra": "iterations: 6364\ncpu: 109113.66923318907 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 911889.0394676107,
            "unit": "ns/iter",
            "extra": "iterations: 764\ncpu: 911948.315445028 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1867136.7837303372,
            "unit": "ns/iter",
            "extra": "iterations: 378\ncpu: 1867197.203703691 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2309.1454416979523,
            "unit": "ns/iter",
            "extra": "iterations: 291131\ncpu: 2306.0440282895916 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10249.744767283226,
            "unit": "ns/iter",
            "extra": "iterations: 68651\ncpu: 10245.842143602029 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 103949.5940980902,
            "unit": "ns/iter",
            "extra": "iterations: 6649\ncpu: 103913.7073244034 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 408634.3134392281,
            "unit": "ns/iter",
            "extra": "iterations: 1715\ncpu: 408614.56793003826 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2299.288143899052,
            "unit": "ns/iter",
            "extra": "iterations: 309125\ncpu: 2270.2089607761013 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6745.190519861801,
            "unit": "ns/iter",
            "extra": "iterations: 104189\ncpu: 6714.777606084699 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42206.67878685067,
            "unit": "ns/iter",
            "extra": "iterations: 16613\ncpu: 42174.45867693113 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 326444.96154241654,
            "unit": "ns/iter",
            "extra": "iterations: 2144\ncpu: 326325.11287318385 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 658888.3312853207,
            "unit": "ns/iter",
            "extra": "iterations: 1064\ncpu: 658823.6240601253 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "64100945bae9c836ca5f74a3e9e1248b417c9b6f",
          "message": "Hopefully fix macos build",
          "timestamp": "2026-05-14T22:30:34-06:00",
          "tree_id": "388f132f05ba3ce6beaeb65e24217737d8abec40",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/64100945bae9c836ca5f74a3e9e1248b417c9b6f"
        },
        "date": 1778819863316,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4019.8716245204278,
            "unit": "ns/iter",
            "extra": "iterations: 172876\ncpu: 4059.2722355907895 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16280.801244188278,
            "unit": "ns/iter",
            "extra": "iterations: 42826\ncpu: 16326.46469434705 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 108946.86674718188,
            "unit": "ns/iter",
            "extra": "iterations: 6078\ncpu: 108998.89190522731 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 925834.2189942702,
            "unit": "ns/iter",
            "extra": "iterations: 734\ncpu: 925941.8228882852 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1847969.8262507876,
            "unit": "ns/iter",
            "extra": "iterations: 372\ncpu: 1847300.462365591 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2269.2086357223616,
            "unit": "ns/iter",
            "extra": "iterations: 303757\ncpu: 2267.1454188713265 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10105.367770761803,
            "unit": "ns/iter",
            "extra": "iterations: 69383\ncpu: 10100.10286380937 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 68970.11022143652,
            "unit": "ns/iter",
            "extra": "iterations: 10212\ncpu: 68940.09596553819 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 400207.80668343767,
            "unit": "ns/iter",
            "extra": "iterations: 1772\ncpu: 400190.23419866216 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2156.658486320282,
            "unit": "ns/iter",
            "extra": "iterations: 319067\ncpu: 2123.6724418363915 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6748.548878453324,
            "unit": "ns/iter",
            "extra": "iterations: 104781\ncpu: 6713.205065796833 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 43547.884068285086,
            "unit": "ns/iter",
            "extra": "iterations: 16176\ncpu: 43510.8411844587 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 336894.78037937504,
            "unit": "ns/iter",
            "extra": "iterations: 2065\ncpu: 336781.29104113585 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 678841.1839785477,
            "unit": "ns/iter",
            "extra": "iterations: 1017\ncpu: 678689.4670599923 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "3248135ec877d8399e56a21076b36442e1d614e1",
          "message": "Hopefully fix macos build",
          "timestamp": "2026-05-14T22:48:50-06:00",
          "tree_id": "ce6ae7ab159e7e5c45bc956229689aad7139b8c0",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/3248135ec877d8399e56a21076b36442e1d614e1"
        },
        "date": 1778820940195,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3982.2418759131783,
            "unit": "ns/iter",
            "extra": "iterations: 175012\ncpu: 4022.6162548857455 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16155.31414103491,
            "unit": "ns/iter",
            "extra": "iterations: 42360\ncpu: 16205.163385267557 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107323.42461136951,
            "unit": "ns/iter",
            "extra": "iterations: 6503\ncpu: 107363.55236044452 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 906604.3066205445,
            "unit": "ns/iter",
            "extra": "iterations: 773\ncpu: 906619.9029754285 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1840773.403313857,
            "unit": "ns/iter",
            "extra": "iterations: 383\ncpu: 1840760.5848563993 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2257.05679380697,
            "unit": "ns/iter",
            "extra": "iterations: 314268\ncpu: 2254.19967670962 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9989.311921157205,
            "unit": "ns/iter",
            "extra": "iterations: 70374\ncpu: 9980.96652172858 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 101951.25988814769,
            "unit": "ns/iter",
            "extra": "iterations: 7691\ncpu: 101957.96866468526 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 395959.68068215554,
            "unit": "ns/iter",
            "extra": "iterations: 1753\ncpu: 395933.87335995014 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2326.719858745962,
            "unit": "ns/iter",
            "extra": "iterations: 306747\ncpu: 2297.230916683922 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6804.456947249191,
            "unit": "ns/iter",
            "extra": "iterations: 103109\ncpu: 6774.31867247561 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42182.327616176444,
            "unit": "ns/iter",
            "extra": "iterations: 16649\ncpu: 42149.18535647592 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 326490.47374552477,
            "unit": "ns/iter",
            "extra": "iterations: 2155\ncpu: 326407.6932713997 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 668833.1238334951,
            "unit": "ns/iter",
            "extra": "iterations: 1022\ncpu: 668660.4980430006 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "538ded3b3de5a2d9e0d99aa5cad0f0aaa4670a22",
          "message": "Fix macos build",
          "timestamp": "2026-05-14T23:00:18-06:00",
          "tree_id": "ce6ae7ab159e7e5c45bc956229689aad7139b8c0",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/538ded3b3de5a2d9e0d99aa5cad0f0aaa4670a22"
        },
        "date": 1778821627841,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4029.0327899545614,
            "unit": "ns/iter",
            "extra": "iterations: 173380\ncpu: 4064.78798592656 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15950.74775621086,
            "unit": "ns/iter",
            "extra": "iterations: 44137\ncpu: 15991.271903389816 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106640.9779428035,
            "unit": "ns/iter",
            "extra": "iterations: 6468\ncpu: 106693.37461348053 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 896695.2686827716,
            "unit": "ns/iter",
            "extra": "iterations: 778\ncpu: 896802.1902313561 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1824783.7029455393,
            "unit": "ns/iter",
            "extra": "iterations: 386\ncpu: 1824704.5362694208 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2345.180627268562,
            "unit": "ns/iter",
            "extra": "iterations: 304486\ncpu: 2340.6830626034302 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9984.990670904734,
            "unit": "ns/iter",
            "extra": "iterations: 70481\ncpu: 9968.894269379343 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 70832.64143613644,
            "unit": "ns/iter",
            "extra": "iterations: 9906\ncpu: 70799.32949727729 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 398346.25319193647,
            "unit": "ns/iter",
            "extra": "iterations: 1770\ncpu: 398300.84802260064 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2165.7050415399162,
            "unit": "ns/iter",
            "extra": "iterations: 327165\ncpu: 2142.055507159414 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6756.0616746415735,
            "unit": "ns/iter",
            "extra": "iterations: 104344\ncpu: 6729.9618377598445 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42191.394353579206,
            "unit": "ns/iter",
            "extra": "iterations: 16612\ncpu: 42161.21881770015 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 326741.8150585136,
            "unit": "ns/iter",
            "extra": "iterations: 2159\ncpu: 326570.8642890065 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 649419.7777883951,
            "unit": "ns/iter",
            "extra": "iterations: 1075\ncpu: 649335.8055814626 ns\nthreads: 1"
          }
        ]
      }
    ],
    "Octarine Engine Macro-Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "09244773702906313f383b89db5784b2fb322501",
          "message": "Update benchmark.yml",
          "timestamp": "2026-05-11T21:10:26-06:00",
          "tree_id": "54536db3ceb47b8cb58050f7f06289d29e46b930",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/09244773702906313f383b89db5784b2fb322501"
        },
        "date": 1778556785553,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 13.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 14.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 604"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.073,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 604"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 604"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.75,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.992,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.196,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Collision System Update [max]",
            "value": 1.381,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 3.879,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.735,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 5.47,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 3.883,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.738,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 5.474,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.325,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.391,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Sort [max]",
            "value": 2.481,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.707,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.171,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Draw [max]",
            "value": 4.556,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Present [p99]",
            "value": 6.753,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Present [max]",
            "value": 11.758,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.775,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.524,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 19.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.255,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 291"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.308,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 291"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.316,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 291"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.223,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.292,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 4.852,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 5.696,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.724,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 0.827,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 1.088,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 1.228,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 0.928,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 0.999,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 1.049,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2676a5363e781349b5cec54a669c5e415a6c4763",
          "message": "Use a struct tag for projectiles",
          "timestamp": "2026-05-11T21:44:18-06:00",
          "tree_id": "5f11b79232c76aa84620f22d1219a7fb001d78c7",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2676a5363e781349b5cec54a669c5e415a6c4763"
        },
        "date": 1778558400898,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 12.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 13.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 1.915,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 2.322,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 2.562,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 602"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 602"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 602"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.232,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.555,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.418,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.473,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 0.945,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.867,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.104,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 0.876,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.918,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.591,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.961,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Collision System Update [max]",
            "value": 1.655,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 0.965,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 1.659,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.589,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 3.921,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 4.811,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 1.107,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.155,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 4.868,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.435,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 8.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 9.865,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.44,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 8.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 9.871,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.357,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.466,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Sort [max]",
            "value": 2.306,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.892,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.247,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.309,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.383,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Present [p99]",
            "value": 6.908,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Present [max]",
            "value": 12.229,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.972,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.606,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 18.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.723,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.432,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.901,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.212,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.239,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.245,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 5.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 5.728,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 5.915,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.245,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 289"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.386,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 289"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.473,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 289"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 294"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 294"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 294"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "152f8e2e2337bbb79e8c74918afc9ec7b8490f6e",
          "message": "Fix projectiles masks spawned by the projectile emitter",
          "timestamp": "2026-05-11T22:25:47-06:00",
          "tree_id": "0edbd2d84978d058fd3bbc0e2ad5375cd6472018",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/152f8e2e2337bbb79e8c74918afc9ec7b8490f6e"
        },
        "date": 1778560536006,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 12.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 13.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 1.805,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 2.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 2.508,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 606"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 606"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 606"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.229,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.262,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.414,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.467,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 2.279,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.235,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 1.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.244,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 1.416,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.946,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 2.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.995,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 1.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 306"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 306"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 2.145,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 306"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.403,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 3.923,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 5.609,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 5.893,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 1.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 2.318,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 7.469,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 20.577,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 7.475,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 20.623,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.373,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.451,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.652,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.499,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.244,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.284,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Present [p99]",
            "value": 6.77,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Present [max]",
            "value": 6.95,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.484,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 10.564,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.168,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.884,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.215,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.235,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.32,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 5.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 5.561,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 5.775,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.235,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 293"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.367,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 293"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.384,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 293"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "ef5d49329d8b293539cb6c4a168adece6b2a0574",
          "message": "Update benchmark for what should be more consistent results",
          "timestamp": "2026-05-11T22:38:58-06:00",
          "tree_id": "14dff3d9af8b2516842bcf434b9bb7c07d377a5b",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/ef5d49329d8b293539cb6c4a168adece6b2a0574"
        },
        "date": 1778561321063,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.861,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 5.089,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 5.401,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 1.113,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.265,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 1.33,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.887,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.213,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.267,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 1.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 7.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 8.121,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 8.792,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.674,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.323,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Render: Present [max]",
            "value": 8.863,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.154,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.251,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 13.761,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 1.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 2.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 2.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 0.84,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 0.978,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 1.23,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 722"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 722"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.051,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 722"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.203,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.23,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.442,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.435,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.568,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 6.343,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.097,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 4.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 4.294,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.486,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.561,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.682,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.962,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.345,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.421,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.45,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.948,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.183,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 1.454,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.954,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 2.189,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.153,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.205,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.24,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.71,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 9.644,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 18.462,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.715,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 9.652,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 18.467,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.353,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.552,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Sort [max]",
            "value": 2.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.644,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.446,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "de5709d23833ca5d9c6cbad913de7525f07e84b2",
          "message": "Render walking optimizations\n\nSingle-pass histogram — Build all 8 byte-histograms in one scan instead of 8 separate scans, eliminating ~3.5MB of redundant reads per sort\nSkip-uniform-byte passes — Detect bytes that are identical across all entries (e.g., shared layer) and skip those radix passes entirely. Cuts 8 passes to ~4 in the stress scene\nGather-based permutation — Replace the cycle-walk (random pointer chasing through 80-byte objects) with a sequential-write gather into a scratch buffer",
          "timestamp": "2026-05-11T22:59:01-06:00",
          "tree_id": "916e67143d9fcf476eb9317af1c9a56c4bdc2ea3",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/de5709d23833ca5d9c6cbad913de7525f07e84b2"
        },
        "date": 1778562531582,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Sort [p95]",
            "value": 0.943,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.206,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [max]",
            "value": 3.668,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 3.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [max]",
            "value": 4.369,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.586,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 3.868,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 5.256,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 0.995,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.159,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 1.618,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.66,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.243,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.799,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.261,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.289,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.452,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 7.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 8.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 11.542,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.334,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Present [max]",
            "value": 9.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.755,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 17.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 2.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 2.618,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 3.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 0.775,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 0.929,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 0.992,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 726"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 726"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 726"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.202,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.228,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.262,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.434,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.59,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 0.802,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.46,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.524,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.612,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.972,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.102,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.681,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.431,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.641,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.176,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 1.436,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.647,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 2.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.192,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 2.866,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.259,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 7.977,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 13.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.263,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 7.981,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 13.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2a557375ca25b71ebc508e7a1e694b1a64bf2631",
          "message": "Optimize Registry::Update and system instrumentation\n\nCameraFollowSystem — Scoped to CameraFollowComponent to avoid O(N) execution (fixed ~4.7ms bottleneck)\n\nGather Boxes — Parallelized collision data gathering using ParallelForEach\n\nQuery Counting — Added GetCount() to ComponentQuery for pre-allocating result vectors\n\nType Prettifying — Improved PrettifyTypeName to trim GCC/Clang leading length prefixes",
          "timestamp": "2026-05-11T23:18:23-06:00",
          "tree_id": "f9e6e49d9442b2255075ee70d57f80d7da9bd485",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2a557375ca25b71ebc508e7a1e694b1a64bf2631"
        },
        "date": 1778563691054,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Gather Boxes [p95]",
            "value": 1.212,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.605,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.864,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.675,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.088,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.298,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.682,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.951,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.16,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.164,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.214,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.174,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.592,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 5.185,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.178,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.597,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 5.191,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.14,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.462,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.757,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.243,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.419,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.369,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.148,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [max]",
            "value": 7.932,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.803,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.684,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 12.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.032,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.093,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.17,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.25,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.268,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.316,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.571,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.306,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 6.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.893,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.957,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.972,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.048,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.226,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.277,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.309,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.442,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.61,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.629,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.119,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.145,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.178,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.464,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.558,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.653,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "ae8731c72154e0a0d311a1fbaedae6a0d6ace2d9",
          "message": "Added GLM SIMD compiler flags",
          "timestamp": "2026-05-12T08:45:03-06:00",
          "tree_id": "2aae62c6a78338b500f18c60ab82709f3925b29e",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/ae8731c72154e0a0d311a1fbaedae6a0d6ace2d9"
        },
        "date": 1778597748648,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 3.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.32,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.376,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.833,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.502,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Present [max]",
            "value": 10.541,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.053,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 13.119,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.084,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.249,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 666"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 666"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 666"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 684"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 684"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 684"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.307,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.345,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.393,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.973,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 4.694,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.628,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 8.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.374,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.682,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.73,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 736"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 736"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.209,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 736"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.224,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.41,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 1.322,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.461,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.589,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "MovementSystem [max]",
            "value": 1.701,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.106,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.31,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.115,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.316,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.485,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.613,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.679,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.251,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.829,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 4.729,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.718,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.209,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [max]",
            "value": 5.56,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.722,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.212,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 5.567,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.109,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.17,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.172,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.746,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 13.965,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.176,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.753,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 13.974,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.267,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "261abb78b3fc52496bf6c5476c918419e150a2e2",
          "message": "Improvements to debug UI.\n\nEnabled docking for igui\nAdded save state for editor layout.\nImproved entity browser component.",
          "timestamp": "2026-05-12T22:12:44-06:00",
          "tree_id": "72bfa64fb67db86e1e4381d0a2f4f393836457f1",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/261abb78b3fc52496bf6c5476c918419e150a2e2"
        },
        "date": 1778646257578,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Gather Boxes [p95]",
            "value": 1.249,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.807,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 2.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.726,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [max]",
            "value": 3.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.756,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.252,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 3.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 624"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 624"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 624"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 670"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 670"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 670"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.96,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.223,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 731"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.175,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.199,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.214,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.787,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 6.841,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.219,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.792,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 6.851,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.977,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.185,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.846,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.346,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.544,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.357,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.448,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.846,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.187,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 12.479,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 16.748,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.287,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.331,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.386,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.479,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 8.759,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.084,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.792,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.918,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.523,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.225,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.283,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.349,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.447,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.624,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [max]",
            "value": 1.271,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.109,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.121,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.467,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.568,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.658,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "6402bedc3d06a118a0ee7dcdb5856ca0b87990df",
          "message": "Improved look of editor windows to be easier to see on different display types",
          "timestamp": "2026-05-12T22:19:46-06:00",
          "tree_id": "de6342f33380ff67c8ce97ea2a5c4973a1ec0386",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/6402bedc3d06a118a0ee7dcdb5856ca0b87990df"
        },
        "date": 1778646918451,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.358,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.408,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.968,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.541,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 18.304,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 660"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 660"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 660"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.273,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.335,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.387,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.964,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.192,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.5,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.33,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 9.366,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.091,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.757,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.897,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.348,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 732"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 732"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.807,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 732"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.22,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.394,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 1.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.447,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.592,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "MovementSystem [max]",
            "value": 2.781,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.153,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.113,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.17,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.48,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.643,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.678,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.246,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.772,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 2.724,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.739,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.214,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Collision System Update [max]",
            "value": 3.121,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.744,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.219,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 3.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.107,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.157,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.245,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.801,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 9.801,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.251,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.806,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 9.81,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.979,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.221,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.693,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.264,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.488,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "a221607cd1f8c572a2094ecaf5b5bd774b95620d",
          "message": "Maintain game aspect ratio in debug editor mode",
          "timestamp": "2026-05-12T22:24:40-06:00",
          "tree_id": "5a363c70ff3cd98a6ba98be1b86c71d9352370cd",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a221607cd1f8c572a2094ecaf5b5bd774b95620d"
        },
        "date": 1778647743107,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.362,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.643,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.147,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 22.599,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.073,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 645"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 645"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 645"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 666"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 666"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 666"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.285,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.33,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.523,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.962,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.265,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 725"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.424,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 6.945,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 11.811,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.462,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.894,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.921,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.994,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.049,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.214,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 2.203,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.463,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.589,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.688,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.166,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.176,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.475,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.594,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.704,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.209,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.365,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.92,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.693,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.951,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.375,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.712,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.153,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.186,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.186,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.501,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 6.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.19,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.505,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 6.403,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.973,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.929,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.419,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [max]",
            "value": 4.488,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "482def5e5a6a6c505030170a51769df61b55a5e4",
          "message": "Add Lua console window and history management",
          "timestamp": "2026-05-12T22:33:36-06:00",
          "tree_id": "17ee815489bcd3680406d0ca5481f5ae397dc62a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/482def5e5a6a6c505030170a51769df61b55a5e4"
        },
        "date": 1778649052094,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 2.614,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.294,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.402,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.36,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.383,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.391,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.811,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.934,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.295,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 593"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 593"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 593"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 675"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 675"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 675"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.281,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.305,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.317,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.95,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.184,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 728"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.313,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 6.768,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.201,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.057,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 6.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.657,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.706,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 730"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 730"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.083,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 730"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.246,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.3,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.342,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.44,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.533,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.584,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.325,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.333,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.47,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.551,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.638,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.221,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.71,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.885,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.698,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.27,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.702,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.147,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.273,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.152,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.172,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.488,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.545,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.493,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.55,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.974,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 365"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "b90c4b5c84dd94d251b562109833d11138f9ad9d",
          "message": "Fix Windows build",
          "timestamp": "2026-05-12T22:37:43-06:00",
          "tree_id": "b3426092d2016fe5c72ae15d4f740a73f5e9a45e",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/b90c4b5c84dd94d251b562109833d11138f9ad9d"
        },
        "date": 1778650631171,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.935,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 1.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.279,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.146,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.263,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.294,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.306,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.427,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.507,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.687,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.115,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.966,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.111,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.127,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.974,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.527,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.588,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.598,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.299,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.793,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 5.743,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.789,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.241,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [max]",
            "value": 6.333,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.793,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 377"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.245,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 377"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 6.341,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 377"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 648"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 648"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 648"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 718"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 718"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 718"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.978,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.996,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 733"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.19,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.282,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 1.661,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.945,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 11.263,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.403,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.95,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 11.277,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.294,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Sort [max]",
            "value": 3.574,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.86,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.408,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.785,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.372,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.416,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.524,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 12.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 14.232,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.097,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.3,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.327,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.387,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.809,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.371,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.86,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 4.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "9d0cc87a140fe800b7ded86e5dd807d779fb7a5b",
          "message": "Fix build workflows",
          "timestamp": "2026-05-14T21:36:57-06:00",
          "tree_id": "415c180937f64d692c27b622b546d0728d30e2c2",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/9d0cc87a140fe800b7ded86e5dd807d779fb7a5b"
        },
        "date": 1778816698829,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.07,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.456,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.615,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.17,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.179,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.462,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.595,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.717,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.241,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.708,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.819,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.72,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.183,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.291,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.737,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 380"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.188,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 380"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.296,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 380"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 628"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 628"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 628"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 667"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 667"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 667"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.227,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.331,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.477,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.812,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.967,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.481,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.816,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.971,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.073,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.173,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.78,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.316,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.407,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.353,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.395,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.416,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.889,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.088,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.338,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.286,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.327,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.335,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.571,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.834,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 4.78,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.729,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.816,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.95,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.265,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.318,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.846,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "6775a29273dc1cd00263b1f8f7c23bc644179810",
          "message": "Support editor and player builds. Macos support",
          "timestamp": "2026-05-14T21:48:38-06:00",
          "tree_id": "b66c6cec2b5b45bc5e85815107628a5309d27c9b",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/6775a29273dc1cd00263b1f8f7c23bc644179810"
        },
        "date": 1778817382184,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Gather Boxes [p95]",
            "value": 1.2,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.733,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.942,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.692,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.397,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.699,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.401,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 667"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 667"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 667"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.188,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.205,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.221,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.236,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.36,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.646,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 5.238,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.364,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.65,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 5.246,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.157,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.216,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.795,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.353,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 4.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.351,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.379,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.392,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.186,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.804,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 19.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.281,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.314,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.634,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.506,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.226,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 9.053,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 6.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.869,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.932,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.996,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.277,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.401,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.613,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.463,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.605,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.164,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.491,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.576,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.623,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "b1c29c587258376dcae372bfc33d78db049520f0",
          "message": "Simplify parameter args and update readme",
          "timestamp": "2026-05-14T22:01:28-06:00",
          "tree_id": "121ffa1d7bb5a717963ff917f43182d690f3efca",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/b1c29c587258376dcae372bfc33d78db049520f0"
        },
        "date": 1778818163197,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 3.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.257,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.432,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.346,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.369,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.402,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.805,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.661,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.637,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 648"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 648"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 648"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 674"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 674"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.28,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 674"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.273,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.3,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.312,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.145,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.949,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.485,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 6.766,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.806,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.779,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.861,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.252,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.314,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.333,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.453,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.642,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.756,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.149,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.109,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.481,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.53,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.58,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.19,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.738,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.683,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.805,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.152,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.685,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 390"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 1.811,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 390"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.155,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 390"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.158,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.215,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.276,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.473,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.283,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.479,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.195,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.309,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "632262380642f78c0b7d1e30c22916c967d31faf",
          "message": "Hopefully fix macos build",
          "timestamp": "2026-05-14T22:21:21-06:00",
          "tree_id": "68d9e72dd57a19b15620ad9d16ed7ad23aea5b5a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/632262380642f78c0b7d1e30c22916c967d31faf"
        },
        "date": 1778819353333,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 2.87,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.391,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.537,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.354,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.385,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.93,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.938,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 16.754,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.104,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 634"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 634"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 634"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 688"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 688"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 688"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.242,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.286,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.338,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.162,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.417,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 738"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.598,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 9.308,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.864,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.919,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.61,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.091,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.107,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.254,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.31,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.727,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.45,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.615,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.657,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.118,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.16,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.476,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.515,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.6,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.2,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.667,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.724,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.657,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.224,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.662,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.229,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.183,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.218,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.351,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.683,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 6.476,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.356,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.688,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 6.482,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.28,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "64100945bae9c836ca5f74a3e9e1248b417c9b6f",
          "message": "Hopefully fix macos build",
          "timestamp": "2026-05-14T22:30:34-06:00",
          "tree_id": "388f132f05ba3ce6beaeb65e24217737d8abec40",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/64100945bae9c836ca5f74a3e9e1248b417c9b6f"
        },
        "date": 1778819917781,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.257,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.384,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.172,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.242,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.283,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.439,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 9.47,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.444,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 5.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 9.477,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.975,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.281,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.676,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.482,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.365,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.41,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.749,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.479,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.782,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 15.476,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.083,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.1,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 658"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 658"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 658"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.287,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.404,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.585,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 8.323,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 4.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.845,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.958,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.305,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.089,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.255,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.295,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 3.475,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.43,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.509,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.928,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.115,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.204,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.106,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.217,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.47,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.572,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.712,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.257,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.68,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.852,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.745,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.354,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.75,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.359,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "3248135ec877d8399e56a21076b36442e1d614e1",
          "message": "Hopefully fix macos build",
          "timestamp": "2026-05-14T22:48:50-06:00",
          "tree_id": "ce6ae7ab159e7e5c45bc956229689aad7139b8c0",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/3248135ec877d8399e56a21076b36442e1d614e1"
        },
        "date": 1778820994515,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.797,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.862,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.879,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.097,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.14,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.272,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.352,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.495,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.459,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.614,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.896,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.404,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.416,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.502,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.591,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.685,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.223,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.701,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.845,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.73,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.182,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.279,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.734,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 383"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.192,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 383"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.283,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 383"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 617"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 617"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 617"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 697"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 697"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 697"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.178,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.295,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.157,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.22,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.257,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.347,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.768,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.964,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.352,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.773,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.971,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.048,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.147,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.23,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.883,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.284,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.401,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.344,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.374,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.404,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.864,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.641,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.787,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.304,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.35,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.367,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.476,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 6.844,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.189,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "538ded3b3de5a2d9e0d99aa5cad0f0aaa4670a22",
          "message": "Fix macos build",
          "timestamp": "2026-05-14T23:00:18-06:00",
          "tree_id": "ce6ae7ab159e7e5c45bc956229689aad7139b8c0",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/538ded3b3de5a2d9e0d99aa5cad0f0aaa4670a22"
        },
        "date": 1778821683121,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 2.679,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.376,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.345,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.386,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.403,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.81,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.639,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 639"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 639"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.049,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 639"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 671"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 671"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 671"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.342,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.394,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.938,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.991,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.407,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.19,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.838,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.933,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.163,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.276,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.376,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.646,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.457,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.626,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [max]",
            "value": 10.361,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.476,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.59,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.638,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.182,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.326,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.86,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.666,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.978,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.264,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.659,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 389"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 1.984,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 389"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.268,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 389"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.175,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.22,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.157,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.629,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 14.787,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.161,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.633,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 14.796,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.995,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.209,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          }
        ]
      }
    ]
  }
}
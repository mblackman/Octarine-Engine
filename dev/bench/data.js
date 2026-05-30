window.BENCHMARK_DATA = {
  "lastUpdate": 1780151495353,
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
          "id": "a1fa1c3a9c5bb8c5fa88fda2efecef41dc793e35",
          "message": "Added render culling to render systems to prevent excessive rendering",
          "timestamp": "2026-05-14T23:10:40-06:00",
          "tree_id": "89bf99872f5e9ad4eae3beb2dc6d2caf62bb4236",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a1fa1c3a9c5bb8c5fa88fda2efecef41dc793e35"
        },
        "date": 1778822255933,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4013.56899400811,
            "unit": "ns/iter",
            "extra": "iterations: 173586\ncpu: 4049.111172560136 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16075.509946858927,
            "unit": "ns/iter",
            "extra": "iterations: 43266\ncpu: 16117.947233394634 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107796.81445602915,
            "unit": "ns/iter",
            "extra": "iterations: 6522\ncpu: 107824.1064090734 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 908838.6894438071,
            "unit": "ns/iter",
            "extra": "iterations: 767\ncpu: 908888.443285538 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1839673.8107855383,
            "unit": "ns/iter",
            "extra": "iterations: 380\ncpu: 1839721.4263158077 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2356.089818307359,
            "unit": "ns/iter",
            "extra": "iterations: 295413\ncpu: 2352.854735573589 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10094.30495059928,
            "unit": "ns/iter",
            "extra": "iterations: 69937\ncpu: 10086.652901895219 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 87284.82982582778,
            "unit": "ns/iter",
            "extra": "iterations: 9368\ncpu: 87261.18488469928 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 400648.0322601663,
            "unit": "ns/iter",
            "extra": "iterations: 1747\ncpu: 400622.70520893217 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2272.593637840754,
            "unit": "ns/iter",
            "extra": "iterations: 310801\ncpu: 2244.197254837212 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6864.969962907133,
            "unit": "ns/iter",
            "extra": "iterations: 102576\ncpu: 6830.4305880551865 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42617.118999996506,
            "unit": "ns/iter",
            "extra": "iterations: 16486\ncpu: 42576.643091092796 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 327499.8845071451,
            "unit": "ns/iter",
            "extra": "iterations: 2137\ncpu: 327416.90266729274 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 682421.4575243925,
            "unit": "ns/iter",
            "extra": "iterations: 1039\ncpu: 682170.4177092855 ns\nthreads: 1"
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
          "id": "45644ec6adfd8b1dfa3355a35eedc03ba8c31801",
          "message": "Standardize naming of entity masks and allow assigning at entity or component level.\nAdded validation to prevent trying to set the entity mask twice.",
          "timestamp": "2026-05-15T09:04:22-06:00",
          "tree_id": "48dfff080b19dc0bbe58f2a8b62dca2d2fba2518",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/45644ec6adfd8b1dfa3355a35eedc03ba8c31801"
        },
        "date": 1778857891691,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3987.7828782188844,
            "unit": "ns/iter",
            "extra": "iterations: 174378\ncpu: 4021.266633405392 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15838.995989760739,
            "unit": "ns/iter",
            "extra": "iterations: 44404\ncpu: 15879.009661290684 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107258.85249960881,
            "unit": "ns/iter",
            "extra": "iterations: 6500\ncpu: 107298.78646153881 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 911944.202686611,
            "unit": "ns/iter",
            "extra": "iterations: 760\ncpu: 911903.8315789511 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1831289.8855607498,
            "unit": "ns/iter",
            "extra": "iterations: 381\ncpu: 1831208.5984251946 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2280.62599662253,
            "unit": "ns/iter",
            "extra": "iterations: 307806\ncpu: 2279.491033312585 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9974.115150086793,
            "unit": "ns/iter",
            "extra": "iterations: 70204\ncpu: 9966.482679046696 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 69009.3645711004,
            "unit": "ns/iter",
            "extra": "iterations: 10125\ncpu: 68973.82093827259 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 404484.5721539193,
            "unit": "ns/iter",
            "extra": "iterations: 1751\ncpu: 404456.88349513884 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2206.288736062275,
            "unit": "ns/iter",
            "extra": "iterations: 322860\ncpu: 2172.0022703333325 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6811.082821423857,
            "unit": "ns/iter",
            "extra": "iterations: 104940\ncpu: 6776.663712600283 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42286.51809037905,
            "unit": "ns/iter",
            "extra": "iterations: 16607\ncpu: 42250.85138796241 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 334040.60503896413,
            "unit": "ns/iter",
            "extra": "iterations: 2130\ncpu: 333916.78873238317 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 658743.9605307982,
            "unit": "ns/iter",
            "extra": "iterations: 1064\ncpu: 658608.4229323003 ns\nthreads: 1"
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
          "id": "7d039d6dde106a8b38db88a560243d6fdc0133e1",
          "message": "Fix char",
          "timestamp": "2026-05-15T09:05:28-06:00",
          "tree_id": "7afde9a120a11510310242c71139c5692401238d",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/7d039d6dde106a8b38db88a560243d6fdc0133e1"
        },
        "date": 1778858366860,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3965.403477698101,
            "unit": "ns/iter",
            "extra": "iterations: 171686\ncpu: 4005.1381242500315 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16058.515521734216,
            "unit": "ns/iter",
            "extra": "iterations: 43485\ncpu: 16105.042865356056 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106054.23255552913,
            "unit": "ns/iter",
            "extra": "iterations: 6583\ncpu: 106096.13671578294 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 915433.6946393481,
            "unit": "ns/iter",
            "extra": "iterations: 755\ncpu: 914544.8715231817 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1823339.1626521596,
            "unit": "ns/iter",
            "extra": "iterations: 383\ncpu: 1823481.2924282134 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2236.8167188906978,
            "unit": "ns/iter",
            "extra": "iterations: 312324\ncpu: 2228.705245194255 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9944.133390716166,
            "unit": "ns/iter",
            "extra": "iterations: 71458\ncpu: 9936.85668504428 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 78214.70676411815,
            "unit": "ns/iter",
            "extra": "iterations: 8191\ncpu: 78194.65694054392 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 400893.77686970984,
            "unit": "ns/iter",
            "extra": "iterations: 1769\ncpu: 400847.75522895664 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2190.237178610592,
            "unit": "ns/iter",
            "extra": "iterations: 320284\ncpu: 2161.399698390068 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6670.61503693244,
            "unit": "ns/iter",
            "extra": "iterations: 105682\ncpu: 6639.459614687889 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42603.648016071056,
            "unit": "ns/iter",
            "extra": "iterations: 16423\ncpu: 42572.96054314247 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 328494.063012465,
            "unit": "ns/iter",
            "extra": "iterations: 2147\ncpu: 328411.44340941135 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 655680.5023988384,
            "unit": "ns/iter",
            "extra": "iterations: 1005\ncpu: 655552.782089606 ns\nthreads: 1"
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
          "id": "d8196fb575d11613beb2d4acb1007c63c0e974d0",
          "message": "Added a name component that shows up in the hierarchy browser",
          "timestamp": "2026-05-15T09:15:17-06:00",
          "tree_id": "430160cbc73d5c54bb97d93460cf81fd313bf987",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/d8196fb575d11613beb2d4acb1007c63c0e974d0"
        },
        "date": 1778858843976,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3996.6736212516234,
            "unit": "ns/iter",
            "extra": "iterations: 171390\ncpu: 4033.377320730735 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16158.65212875468,
            "unit": "ns/iter",
            "extra": "iterations: 43697\ncpu: 16203.334851363083 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 108101.96083986152,
            "unit": "ns/iter",
            "extra": "iterations: 6490\ncpu: 108161.14838212804 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 935324.4424501592,
            "unit": "ns/iter",
            "extra": "iterations: 774\ncpu: 935205.3992248062 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1852256.452464291,
            "unit": "ns/iter",
            "extra": "iterations: 381\ncpu: 1852380.5748031824 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2255.8973238389462,
            "unit": "ns/iter",
            "extra": "iterations: 311913\ncpu: 2252.2418559023886 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10050.597426226479,
            "unit": "ns/iter",
            "extra": "iterations: 69652\ncpu: 10044.575475221129 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 87313.51388299538,
            "unit": "ns/iter",
            "extra": "iterations: 9085\ncpu: 87277.39229497971 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 396852.86398821644,
            "unit": "ns/iter",
            "extra": "iterations: 1774\ncpu: 396822.655016902 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2342.776737930503,
            "unit": "ns/iter",
            "extra": "iterations: 305905\ncpu: 2305.214099150281 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6826.945943637479,
            "unit": "ns/iter",
            "extra": "iterations: 103719\ncpu: 6788.354669828508 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42179.96729272521,
            "unit": "ns/iter",
            "extra": "iterations: 16617\ncpu: 42145.11000782388 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 326261.0692501456,
            "unit": "ns/iter",
            "extra": "iterations: 2153\ncpu: 326133.84300976427 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653273.8639859824,
            "unit": "ns/iter",
            "extra": "iterations: 1077\ncpu: 653117.931290583 ns\nthreads: 1"
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
          "id": "e531de6eabb08bbacce18aec7cbbe7ea860a6710",
          "message": "Added naming support to projectiles",
          "timestamp": "2026-05-15T09:23:05-06:00",
          "tree_id": "36a42fa199b223ea998697d9b0b109d5964d3d2c",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/e531de6eabb08bbacce18aec7cbbe7ea860a6710"
        },
        "date": 1778859318085,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3957.1263110195823,
            "unit": "ns/iter",
            "extra": "iterations: 175883\ncpu: 3993.2481081172236 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15741.857779821326,
            "unit": "ns/iter",
            "extra": "iterations: 43559\ncpu: 15783.281044102163 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107938.18733137989,
            "unit": "ns/iter",
            "extra": "iterations: 6549\ncpu: 107983.90074820527 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 899427.3685959842,
            "unit": "ns/iter",
            "extra": "iterations: 773\ncpu: 899561.543337631 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1826420.777742374,
            "unit": "ns/iter",
            "extra": "iterations: 383\ncpu: 1826602.8903394113 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2288.1397770695844,
            "unit": "ns/iter",
            "extra": "iterations: 308652\ncpu: 2284.5361410264513 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9972.057967738789,
            "unit": "ns/iter",
            "extra": "iterations: 69695\ncpu: 9967.233474422559 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 80538.79153057623,
            "unit": "ns/iter",
            "extra": "iterations: 8636\ncpu: 80488.51227419953 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 398858.80277407443,
            "unit": "ns/iter",
            "extra": "iterations: 1760\ncpu: 398846.24090908177 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2356.2925775782846,
            "unit": "ns/iter",
            "extra": "iterations: 303076\ncpu: 2325.3838938055374 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6768.176366132475,
            "unit": "ns/iter",
            "extra": "iterations: 104258\ncpu: 6735.659709554733 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41936.24572837484,
            "unit": "ns/iter",
            "extra": "iterations: 16631\ncpu: 41899.143527151115 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 324462.48856404563,
            "unit": "ns/iter",
            "extra": "iterations: 2163\ncpu: 324331.9519186722 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 646011.0091071986,
            "unit": "ns/iter",
            "extra": "iterations: 1083\ncpu: 645928.5364727438 ns\nthreads: 1"
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
          "id": "a9b5acbfb22c72cfdb7872c2a12d52641fbd466b",
          "message": "Add audio source/sink, pool & deferred commands\n\nIntroduce AudioSourceComponent and AudioSinkComponent and wire audio into the ECS/Lua stack. Revamp AudioSystem to use a managed track pool with generation-tagged slots, dynamic initial/max sizing from EngineOptions, safer acquisition/ownership checks, proper mixer lifecycle, and per-entity sink management (playOnSpawn, loop, despawnOnFinish). Add deferred function channel and templated AddComponent to CommandBuffer, and ensure system wrappers playback command buffers after system execution. Update Registry playback to handle deferred component adds. Expose audio_source creation in the Lua component factory and register the AudioSystem via the Registry in Game::Setup; Init now reports failure instead of assuming success. These changes enable safe, deferred component mutations from systems, reduce per-frame audio retries, and prevent pool reuse races.",
          "timestamp": "2026-05-15T09:46:43-06:00",
          "tree_id": "7dd89baef189dfdd9ad8f46d953957278095d914",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a9b5acbfb22c72cfdb7872c2a12d52641fbd466b"
        },
        "date": 1778860418039,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4017.5618949863356,
            "unit": "ns/iter",
            "extra": "iterations: 172545\ncpu: 4055.5292996030234 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15957.821179474817,
            "unit": "ns/iter",
            "extra": "iterations: 42333\ncpu: 16005.538657784908 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107681.3338582936,
            "unit": "ns/iter",
            "extra": "iterations: 6456\ncpu: 107729.4165117745 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 898036.2216998202,
            "unit": "ns/iter",
            "extra": "iterations: 779\ncpu: 898038.2618741974 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1798555.263402787,
            "unit": "ns/iter",
            "extra": "iterations: 385\ncpu: 1798686.8181818423 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2380.569882056039,
            "unit": "ns/iter",
            "extra": "iterations: 294647\ncpu: 2377.293038789467 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10001.530814089707,
            "unit": "ns/iter",
            "extra": "iterations: 69867\ncpu: 9997.390069701956 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 100526.91556135728,
            "unit": "ns/iter",
            "extra": "iterations: 7005\ncpu: 100526.40599571675 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 396569.6579607998,
            "unit": "ns/iter",
            "extra": "iterations: 1761\ncpu: 396565.3617262832 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2359.660219884403,
            "unit": "ns/iter",
            "extra": "iterations: 290673\ncpu: 2329.9965459472005 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 7008.873303334912,
            "unit": "ns/iter",
            "extra": "iterations: 101554\ncpu: 6974.111526874914 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41926.76553153663,
            "unit": "ns/iter",
            "extra": "iterations: 16731\ncpu: 41891.50337697924 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 323175.4917410852,
            "unit": "ns/iter",
            "extra": "iterations: 2168\ncpu: 323062.54935427697 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 651707.7435649211,
            "unit": "ns/iter",
            "extra": "iterations: 1085\ncpu: 651550.2617511956 ns\nthreads: 1"
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
          "id": "a8984cf57607ecab7b050ee486761c02a88e0c92",
          "message": "Strip out unused code and optimize LTO for dependencies in release builds for smaller executables",
          "timestamp": "2026-05-16T11:07:52-06:00",
          "tree_id": "e6b6f981501c2018eb3a64bbd6b9507af190c981",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a8984cf57607ecab7b050ee486761c02a88e0c92"
        },
        "date": 1778951700009,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3898.075504002126,
            "unit": "ns/iter",
            "extra": "iterations: 177067\ncpu: 3926.5662602287243 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16235.940995077905,
            "unit": "ns/iter",
            "extra": "iterations: 43873\ncpu: 16273.5404234952 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106429.3953666772,
            "unit": "ns/iter",
            "extra": "iterations: 6486\ncpu: 106477.16913352055 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 904826.4831877672,
            "unit": "ns/iter",
            "extra": "iterations: 767\ncpu: 904908.5293350439 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1851134.5193378236,
            "unit": "ns/iter",
            "extra": "iterations: 380\ncpu: 1851076.881578942 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2249.4372263024616,
            "unit": "ns/iter",
            "extra": "iterations: 300519\ncpu: 2238.463261889719 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10026.574611016678,
            "unit": "ns/iter",
            "extra": "iterations: 70322\ncpu: 10017.494525182723 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 69489.46216832321,
            "unit": "ns/iter",
            "extra": "iterations: 9088\ncpu: 69461.66978433453 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 394894.6721776228,
            "unit": "ns/iter",
            "extra": "iterations: 1774\ncpu: 394877.08342729637 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2365.8929624771154,
            "unit": "ns/iter",
            "extra": "iterations: 297316\ncpu: 2333.494352808937 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6967.008915564222,
            "unit": "ns/iter",
            "extra": "iterations: 100950\ncpu: 6935.070916290931 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 43785.41687802865,
            "unit": "ns/iter",
            "extra": "iterations: 16038\ncpu: 43750.01639855508 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 336732.13774101366,
            "unit": "ns/iter",
            "extra": "iterations: 2067\ncpu: 336636.19545230817 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 694837.5253827422,
            "unit": "ns/iter",
            "extra": "iterations: 1001\ncpu: 694686.3306693297 ns\nthreads: 1"
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
          "id": "7a2184f940e12d81919a9f198189d13e0f7cad3c",
          "message": "Specify media types to reduce build size a smidge",
          "timestamp": "2026-05-16T11:15:30-06:00",
          "tree_id": "343754e5eb93492bfcb4101117b1a5638865b31a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/7a2184f940e12d81919a9f198189d13e0f7cad3c"
        },
        "date": 1778952146845,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3979.7652571171543,
            "unit": "ns/iter",
            "extra": "iterations: 174515\ncpu: 4010.025745637817 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15772.382699843776,
            "unit": "ns/iter",
            "extra": "iterations: 43777\ncpu: 15816.164195811076 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 108125.03996245253,
            "unit": "ns/iter",
            "extra": "iterations: 6513\ncpu: 108163.27452786334 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 905740.8682518192,
            "unit": "ns/iter",
            "extra": "iterations: 770\ncpu: 905814.048051959 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1838595.7571138677,
            "unit": "ns/iter",
            "extra": "iterations: 380\ncpu: 1838648.0473684478 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2301.6217225821874,
            "unit": "ns/iter",
            "extra": "iterations: 301672\ncpu: 2300.14583720025 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9867.929618783886,
            "unit": "ns/iter",
            "extra": "iterations: 71045\ncpu: 9858.328031529174 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 86214.49827303797,
            "unit": "ns/iter",
            "extra": "iterations: 9833\ncpu: 86191.07261264275 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 393686.47815472423,
            "unit": "ns/iter",
            "extra": "iterations: 1761\ncpu: 393657.51220897195 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2188.2899501307397,
            "unit": "ns/iter",
            "extra": "iterations: 317803\ncpu: 2159.4830067693842 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6545.86753404954,
            "unit": "ns/iter",
            "extra": "iterations: 106996\ncpu: 6509.35295711908 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41201.32809648755,
            "unit": "ns/iter",
            "extra": "iterations: 16971\ncpu: 41158.32803017536 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 323347.2776448169,
            "unit": "ns/iter",
            "extra": "iterations: 2201\ncpu: 323222.87414814165 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 652874.7349997776,
            "unit": "ns/iter",
            "extra": "iterations: 1102\ncpu: 652805.5635208663 ns\nthreads: 1"
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
          "id": "a515d347952869b4123da412ac9f01519a36c6bf",
          "message": "Fix macos build error",
          "timestamp": "2026-05-16T11:16:33-06:00",
          "tree_id": "f6bba67fed2cfe01e52fde5c8becbbf63b9841d2",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a515d347952869b4123da412ac9f01519a36c6bf"
        },
        "date": 1778952584410,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3897.410118871945,
            "unit": "ns/iter",
            "extra": "iterations: 179438\ncpu: 3926.3541836179743 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15998.682494539928,
            "unit": "ns/iter",
            "extra": "iterations: 43055\ncpu: 16037.839972127382 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 109067.88200287375,
            "unit": "ns/iter",
            "extra": "iterations: 6471\ncpu: 109111.40689228762 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 909348.2697670807,
            "unit": "ns/iter",
            "extra": "iterations: 769\ncpu: 909451.1911573455 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1852989.72110462,
            "unit": "ns/iter",
            "extra": "iterations: 381\ncpu: 1852958.1364829594 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2327.5055920648288,
            "unit": "ns/iter",
            "extra": "iterations: 297413\ncpu: 2325.4136772765346 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10063.377494354885,
            "unit": "ns/iter",
            "extra": "iterations: 69477\ncpu: 10055.049771870035 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 69120.86569622376,
            "unit": "ns/iter",
            "extra": "iterations: 10129\ncpu: 69072.1104748729 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 395084.9459790383,
            "unit": "ns/iter",
            "extra": "iterations: 1772\ncpu: 395065.49097065494 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2361.337652588778,
            "unit": "ns/iter",
            "extra": "iterations: 299544\ncpu: 2329.7162052996837 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6729.669781755264,
            "unit": "ns/iter",
            "extra": "iterations: 105148\ncpu: 6693.784389619045 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41081.110497139576,
            "unit": "ns/iter",
            "extra": "iterations: 17032\ncpu: 41046.818752956446 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 315725.5268234823,
            "unit": "ns/iter",
            "extra": "iterations: 2226\ncpu: 315641.13522013044 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 634951.2167624303,
            "unit": "ns/iter",
            "extra": "iterations: 1109\ncpu: 634847.0063120372 ns\nthreads: 1"
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
          "id": "1c189fe8b8fba5764ce7641a56eff89925e5a9e9",
          "message": "Move all build artifacts to build/ directory",
          "timestamp": "2026-05-16T11:47:23-06:00",
          "tree_id": "99acc612eafe6d5e790615e0e47ae9b28d002706",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/1c189fe8b8fba5764ce7641a56eff89925e5a9e9"
        },
        "date": 1778954031014,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3977.483265846236,
            "unit": "ns/iter",
            "extra": "iterations: 177194\ncpu: 4006.3881169794518 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15689.219031792029,
            "unit": "ns/iter",
            "extra": "iterations: 43569\ncpu: 15726.225986366715 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107939.5621469278,
            "unit": "ns/iter",
            "extra": "iterations: 6526\ncpu: 107978.4567882335 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 901571.5985487287,
            "unit": "ns/iter",
            "extra": "iterations: 780\ncpu: 901646.4948717912 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1851925.5483805821,
            "unit": "ns/iter",
            "extra": "iterations: 382\ncpu: 1851586.816753925 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2292.457912821884,
            "unit": "ns/iter",
            "extra": "iterations: 308220\ncpu: 2284.555788073283 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10040.493362469007,
            "unit": "ns/iter",
            "extra": "iterations: 69736\ncpu: 10033.344269819574 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 101136.82585125184,
            "unit": "ns/iter",
            "extra": "iterations: 6866\ncpu: 101144.75138362696 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 396404.13168430794,
            "unit": "ns/iter",
            "extra": "iterations: 1781\ncpu: 396380.2156092035 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2162.8933691475336,
            "unit": "ns/iter",
            "extra": "iterations: 328612\ncpu: 2131.2164771780685 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6694.564118393214,
            "unit": "ns/iter",
            "extra": "iterations: 104738\ncpu: 6657.196585769913 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 43085.33420652376,
            "unit": "ns/iter",
            "extra": "iterations: 16319\ncpu: 43049.72908878798 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 318990.10065139644,
            "unit": "ns/iter",
            "extra": "iterations: 2198\ncpu: 318808.04822562577 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 640267.2667469502,
            "unit": "ns/iter",
            "extra": "iterations: 1118\ncpu: 640172.9588551361 ns\nthreads: 1"
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
          "id": "9bb7838427c2744671ed0b1e3121218228538d10",
          "message": "Break up TransformComponent.h into multiple parts.\nThis change breaks apart the different fields of a TransformComponent.h into different components to reduce unused fields for such a common component.\nUpdated all systems and Lua loading.",
          "timestamp": "2026-05-16T15:57:20-06:00",
          "tree_id": "0e0f2d582d38490fb6ff3bb045cea68837f8e2b4",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/9bb7838427c2744671ed0b1e3121218228538d10"
        },
        "date": 1778969030862,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3915.3616049709794,
            "unit": "ns/iter",
            "extra": "iterations: 178554\ncpu: 3953.1514051771615 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15888.678110739073,
            "unit": "ns/iter",
            "extra": "iterations: 43252\ncpu: 15936.240520669899 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106906.8004104004,
            "unit": "ns/iter",
            "extra": "iterations: 6549\ncpu: 106951.76729271881 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 906607.412784414,
            "unit": "ns/iter",
            "extra": "iterations: 781\ncpu: 906660.9257362491 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1826811.1710441518,
            "unit": "ns/iter",
            "extra": "iterations: 382\ncpu: 1826745.2329842832 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2220.444989697375,
            "unit": "ns/iter",
            "extra": "iterations: 305708\ncpu: 2217.502139295769 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9902.027980816814,
            "unit": "ns/iter",
            "extra": "iterations: 70404\ncpu: 9890.674862224461 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 68616.7757425366,
            "unit": "ns/iter",
            "extra": "iterations: 10243\ncpu: 68597.66689447305 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 390529.4796025287,
            "unit": "ns/iter",
            "extra": "iterations: 1790\ncpu: 390506.5167597868 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2125.854088807594,
            "unit": "ns/iter",
            "extra": "iterations: 335298\ncpu: 2081.5941401368345 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6482.4691117271495,
            "unit": "ns/iter",
            "extra": "iterations: 109099\ncpu: 6437.445760277187 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40698.55556134125,
            "unit": "ns/iter",
            "extra": "iterations: 17293\ncpu: 40641.8210258617 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316363.67610980215,
            "unit": "ns/iter",
            "extra": "iterations: 2210\ncpu: 316285.78190049966 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653884.5603049637,
            "unit": "ns/iter",
            "extra": "iterations: 1070\ncpu: 653704.0317756801 ns\nthreads: 1"
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
          "id": "d1929920d4a7af05f6d22a253cd0de3bc97fa377",
          "message": "Added support for optional components in queries.\nFixed some issues with copying components in queries that caused unneeded copying.",
          "timestamp": "2026-05-16T19:04:49-06:00",
          "tree_id": "f0c17789c8932d3c62b4b83374341bfd68294112",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/d1929920d4a7af05f6d22a253cd0de3bc97fa377"
        },
        "date": 1778980280686,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3917.6322172472746,
            "unit": "ns/iter",
            "extra": "iterations: 176468\ncpu: 3950.169447152027 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15898.472953044695,
            "unit": "ns/iter",
            "extra": "iterations: 43881\ncpu: 15945.118365579001 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107468.08195419119,
            "unit": "ns/iter",
            "extra": "iterations: 6508\ncpu: 107521.80854333035 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 899975.0461720321,
            "unit": "ns/iter",
            "extra": "iterations: 781\ncpu: 900109.845070414 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1819049.2791208355,
            "unit": "ns/iter",
            "extra": "iterations: 385\ncpu: 1819152.8181818104 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2316.699237948704,
            "unit": "ns/iter",
            "extra": "iterations: 314536\ncpu: 2310.9441462987184 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9991.455055142525,
            "unit": "ns/iter",
            "extra": "iterations: 71029\ncpu: 9981.106336848161 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 94524.08373379188,
            "unit": "ns/iter",
            "extra": "iterations: 7028\ncpu: 94502.40993168665 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 389524.67209344247,
            "unit": "ns/iter",
            "extra": "iterations: 1799\ncpu: 389524.0694830478 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2325.7463497210106,
            "unit": "ns/iter",
            "extra": "iterations: 304156\ncpu: 2302.6148982735067 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6694.839616521696,
            "unit": "ns/iter",
            "extra": "iterations: 106702\ncpu: 6666.75342542023 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41057.892574026104,
            "unit": "ns/iter",
            "extra": "iterations: 17185\ncpu: 41023.014082059526 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 315000.6253895422,
            "unit": "ns/iter",
            "extra": "iterations: 2212\ncpu: 314899.96880650014 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 631516.6220951765,
            "unit": "ns/iter",
            "extra": "iterations: 1114\ncpu: 631458.9362656852 ns\nthreads: 1"
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
          "id": "2be94b328fc19520a5e7560703c88ddd4a9aebdc",
          "message": "Lua-driven InputSystem; split MovementSystem into focused systems; drop KeyboardControlSystem\n\nAdds InputSystem (header-only, owned by Registry) that exposes a global `input` table\nto Lua: polling (is_key_down/pressed/released, is_mouse_*, mouse_position, mouse_wheel),\naction bindings (bind/unbind, is_action_down/pressed/released), and callbacks\n(on_key_down/up, on_mouse_down/up, on_mouse_wheel). Subscribes to KeyInputEvent,\nMouseInputEvent, and a new MouseWheelEvent emitted from Game::ProcessInput.\n\nMovementSystem is split by responsibility: VelocityIntegrationSystem (position += velocity*dt),\nOffScreenDespawnSystem (despawns non-player entities outside the playable area), and\nObstacleBounceSystem (event-driven enemy<->obstacle reflect). KeyboardControlSystem,\nMovementSystem, and KeyboardControlComponent are removed; player movement and clamping\nmove to Lua via input.is_action_* + get/set_position + get_game_map_dimensions.\n\nProjectileEmitSystem loses its per-frame system pass and the SPACE auto-fire hook; it\nnow owns only the projectile pool and a public Fire(registry, entity, direction).\nGameplay drives shots via a new fire_projectile(entity, dx, dy) Lua binding. Auto-fire\nfor enemies moves to Lua scripts (see example project's lib/auto_fire.lua).\n\nScriptSystem is trimmed to ScriptComponent dispatch; key polling state and bindings\nmove to InputSystem. Adds get_game_map_dimensions() getter. InputSystem.ResetLuaState\nruns on StopScene/LoadScene to drop stale callbacks/actions.\n\nDocs: removes keyboard_controller component reference; rewrites the player example to\nuse the input table; adds a full input-table API reference.",
          "timestamp": "2026-05-16T21:47:48-06:00",
          "tree_id": "f95786a0bcb065467239575b4859f512622e4672",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2be94b328fc19520a5e7560703c88ddd4a9aebdc"
        },
        "date": 1778990182894,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4234.045494338838,
            "unit": "ns/iter",
            "extra": "iterations: 164412\ncpu: 4263.736035082363 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16543.69456547889,
            "unit": "ns/iter",
            "extra": "iterations: 41514\ncpu: 16584.660885484307 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 111983.46033739441,
            "unit": "ns/iter",
            "extra": "iterations: 6330\ncpu: 112030.03380726877 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 971582.6706898293,
            "unit": "ns/iter",
            "extra": "iterations: 745\ncpu: 971352.6040268439 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1936393.2370544302,
            "unit": "ns/iter",
            "extra": "iterations: 363\ncpu: 1935838.685950403 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2920.4227184116917,
            "unit": "ns/iter",
            "extra": "iterations: 237311\ncpu: 2915.4395287205202 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 14783.305756958169,
            "unit": "ns/iter",
            "extra": "iterations: 46662\ncpu: 14765.987077281652 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 108862.33623277996,
            "unit": "ns/iter",
            "extra": "iterations: 6418\ncpu: 108838.24898721483 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 541200.9016705039,
            "unit": "ns/iter",
            "extra": "iterations: 1293\ncpu: 541137.6697602984 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2384.054992469499,
            "unit": "ns/iter",
            "extra": "iterations: 297610\ncpu: 2354.1154463877756 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6733.772142927282,
            "unit": "ns/iter",
            "extra": "iterations: 104813\ncpu: 6700.0402526360285 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41223.70026791738,
            "unit": "ns/iter",
            "extra": "iterations: 16856\ncpu: 41183.11681298483 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 317328.3774333202,
            "unit": "ns/iter",
            "extra": "iterations: 2209\ncpu: 317232.6758714339 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 630790.4534142204,
            "unit": "ns/iter",
            "extra": "iterations: 1114\ncpu: 630714.6418312716 ns\nthreads: 1"
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
          "id": "7f834012b877105d2054600ee8b3474934bf88d1",
          "message": "Remove display health system and add related components to Lua support",
          "timestamp": "2026-05-17T11:09:59-06:00",
          "tree_id": "b76bb375a84cddc448c1732c9096237985ebccb9",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/7f834012b877105d2054600ee8b3474934bf88d1"
        },
        "date": 1779038195048,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4003.7312914271374,
            "unit": "ns/iter",
            "extra": "iterations: 176064\ncpu: 4033.9967341421175 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15929.27398752416,
            "unit": "ns/iter",
            "extra": "iterations: 44340\ncpu: 15971.172507894187 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 110307.77983366286,
            "unit": "ns/iter",
            "extra": "iterations: 6439\ncpu: 110353.89951856101 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 910887.7217910346,
            "unit": "ns/iter",
            "extra": "iterations: 773\ncpu: 910974.1021992245 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1888715.8884963656,
            "unit": "ns/iter",
            "extra": "iterations: 364\ncpu: 1888596.9093406363 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2365.5658151644802,
            "unit": "ns/iter",
            "extra": "iterations: 299360\ncpu: 2355.6877672366727 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10081.338393171063,
            "unit": "ns/iter",
            "extra": "iterations: 69524\ncpu: 10065.352511363735 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 89714.74251798887,
            "unit": "ns/iter",
            "extra": "iterations: 6979\ncpu: 89700.44949133598 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 388318.6897843268,
            "unit": "ns/iter",
            "extra": "iterations: 1807\ncpu: 388287.1090204776 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2122.77191908354,
            "unit": "ns/iter",
            "extra": "iterations: 336887\ncpu: 2088.1394889082753 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6493.034880420906,
            "unit": "ns/iter",
            "extra": "iterations: 108749\ncpu: 6451.444427066773 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41546.037497253026,
            "unit": "ns/iter",
            "extra": "iterations: 16895\ncpu: 41498.250014786616 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 317742.7390459568,
            "unit": "ns/iter",
            "extra": "iterations: 2194\ncpu: 317621.8573382244 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 638502.4074660043,
            "unit": "ns/iter",
            "extra": "iterations: 1108\ncpu: 638290.5893501836 ns\nthreads: 1"
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
          "id": "52df981b386d9959be4c9c4fbdb8d595756bf2c4",
          "message": "Add new Lua Binding abstraction to make adding new bindings straightforward. Migrate existing bindings.",
          "timestamp": "2026-05-17T11:36:16-06:00",
          "tree_id": "d6d15b2c693fdd02db318ff34d37ce1f3fd944fc",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/52df981b386d9959be4c9c4fbdb8d595756bf2c4"
        },
        "date": 1779039772944,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3974.2229876058054,
            "unit": "ns/iter",
            "extra": "iterations: 174976\ncpu: 4002.5589852325297 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16234.555054476174,
            "unit": "ns/iter",
            "extra": "iterations: 43385\ncpu: 16270.238515614812 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 109456.96617246505,
            "unit": "ns/iter",
            "extra": "iterations: 6309\ncpu: 109496.29608496117 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 908997.4612819004,
            "unit": "ns/iter",
            "extra": "iterations: 771\ncpu: 909086.4643320323 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1846142.7023130765,
            "unit": "ns/iter",
            "extra": "iterations: 381\ncpu: 1846195.1469816342 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2287.843262935874,
            "unit": "ns/iter",
            "extra": "iterations: 301784\ncpu: 2281.0203854415167 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9874.314389876492,
            "unit": "ns/iter",
            "extra": "iterations: 70787\ncpu: 9871.206083036628 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 74208.83871633103,
            "unit": "ns/iter",
            "extra": "iterations: 6971\ncpu: 74170.46377850234 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 392797.9754399336,
            "unit": "ns/iter",
            "extra": "iterations: 1793\ncpu: 392782.9609593325 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2351.1069317791043,
            "unit": "ns/iter",
            "extra": "iterations: 301851\ncpu: 2322.236192691267 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6721.006457881719,
            "unit": "ns/iter",
            "extra": "iterations: 103314\ncpu: 6685.685347580876 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41144.14269009667,
            "unit": "ns/iter",
            "extra": "iterations: 17070\ncpu: 41109.10538956838 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316004.4072592016,
            "unit": "ns/iter",
            "extra": "iterations: 2210\ncpu: 315885.4099548112 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 634617.5699954119,
            "unit": "ns/iter",
            "extra": "iterations: 1105\ncpu: 634522.8606334598 ns\nthreads: 1"
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
          "id": "6f35a51f22747490b8f62972aad577297a289de4",
          "message": "Add rotation/scale Lua bindings and wire rotation through rendering and collision\n\nExpose PositionComponent and RotationComponent via LuaComponentRegistry\nso registry.has_*/get_* and the entity-table component keys work for the\nfull local transform triplet.\n\nPipe GlobalTransformComponent.rotation through SquareCommand and render\nrotated quads via SDL_RenderGeometry. Draw rotated box colliders as a\nwireframe OBB. Promote CollisionSystem boxes to oriented bounding boxes:\nbroadphase still uses an AABB enclosing the OBB so the median-cut sweep\nis unchanged; narrowphase adds SAT over the four face normals when at\nleast one box is rotated.",
          "timestamp": "2026-05-18T22:08:38-06:00",
          "tree_id": "f8e7d49ddd9a31fc1af58cb7eb79eb9602b79c4e",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/6f35a51f22747490b8f62972aad577297a289de4"
        },
        "date": 1779164223941,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3901.9913082003686,
            "unit": "ns/iter",
            "extra": "iterations: 178458\ncpu: 3933.3765143615765 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15928.168682734817,
            "unit": "ns/iter",
            "extra": "iterations: 43874\ncpu: 15970.099329899718 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 108717.07926722962,
            "unit": "ns/iter",
            "extra": "iterations: 6534\ncpu: 108768.41322313757 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 901824.448479658,
            "unit": "ns/iter",
            "extra": "iterations: 772\ncpu: 901876.7499999907 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1839532.4937960343,
            "unit": "ns/iter",
            "extra": "iterations: 383\ncpu: 1839190.9556135882 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2314.3771259390996,
            "unit": "ns/iter",
            "extra": "iterations: 294623\ncpu: 2307.2132759488136 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9909.904071434965,
            "unit": "ns/iter",
            "extra": "iterations: 70991\ncpu: 9901.998506855047 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 69345.39533292151,
            "unit": "ns/iter",
            "extra": "iterations: 10230\ncpu: 69314.67956988975 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 392584.06498581,
            "unit": "ns/iter",
            "extra": "iterations: 1786\ncpu: 392546.264837612 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2304.039708926783,
            "unit": "ns/iter",
            "extra": "iterations: 304998\ncpu: 2274.985291709063 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6656.306126061573,
            "unit": "ns/iter",
            "extra": "iterations: 105632\ncpu: 6623.044872775737 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40854.836722431086,
            "unit": "ns/iter",
            "extra": "iterations: 17057\ncpu: 40810.61528989688 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 315833.02729884494,
            "unit": "ns/iter",
            "extra": "iterations: 2194\ncpu: 315695.8103919574 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653030.7226415685,
            "unit": "ns/iter",
            "extra": "iterations: 1077\ncpu: 652885.7465180971 ns\nthreads: 1"
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
          "id": "2c630ba63d71586571fed8b4900ab392558241bc",
          "message": "Add declarative Lua free-function modules; add HealthComponent methods\n\nMirror the LuaBinding<T> component pattern for global free functions.\nEach domain gets a LuaModuleBinding<M> specialization owning its\ninstall(sol::state&, Game&); RegisterAllModules.cpp drives them in one\nlist. Migrates the hand-wired blocks out of ScriptSystem: globals split\ninto Log/Io/Assets/Audio/Entity/Scene/Game modules, EntityModule owns\nthe registry table + per-component accessor loop. Global names unchanged\nso existing scripts keep working.\n\nScriptSystem::CreateLuaBindings now only binds primitive + component\nusertypes and drops its Game param. Game::Setup calls CreateLuaBindings\n-> RegisterAllLuaModules -> bindAll. ProjectileEmitSystem (and\nEntityPoolManager) Set+Init early so GameModule binds fire_projectile\nagainst the live instance during module install.\n\nHealthComponent gains damage/heal/isDead/fraction. current_health binds\nas a clamping sol::property so Lua field writes cannot bypass invariants;\nis_dead/fraction expose as read-only properties. LuaBinding.h documents\nthe methods-in-bindUsertype pattern and the own-fields-only rule.",
          "timestamp": "2026-05-23T11:31:11-06:00",
          "tree_id": "b12caa008feedef3e0c242dbee12e3b125ffadd2",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2c630ba63d71586571fed8b4900ab392558241bc"
        },
        "date": 1779558099715,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3917.3490051105896,
            "unit": "ns/iter",
            "extra": "iterations: 178664\ncpu: 3958.2840863298584 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15778.889177150457,
            "unit": "ns/iter",
            "extra": "iterations: 43472\ncpu: 15824.512973867906 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107784.58586454813,
            "unit": "ns/iter",
            "extra": "iterations: 6502\ncpu: 107829.35450631237 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 912750.5654760171,
            "unit": "ns/iter",
            "extra": "iterations: 768\ncpu: 912841.1953125022 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1844921.2657287717,
            "unit": "ns/iter",
            "extra": "iterations: 380\ncpu: 1844959.8131578658 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2320.9929953096516,
            "unit": "ns/iter",
            "extra": "iterations: 304834\ncpu: 2315.0888319551646 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10159.026605733668,
            "unit": "ns/iter",
            "extra": "iterations: 69403\ncpu: 10150.088108582338 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 71146.50646537163,
            "unit": "ns/iter",
            "extra": "iterations: 10036\ncpu: 71109.66311279732 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 398468.7991729309,
            "unit": "ns/iter",
            "extra": "iterations: 1753\ncpu: 398411.1534512222 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2167.4397725989497,
            "unit": "ns/iter",
            "extra": "iterations: 326166\ncpu: 2129.8047313313336 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6545.953206522763,
            "unit": "ns/iter",
            "extra": "iterations: 106794\ncpu: 6502.877792763082 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40704.90146375884,
            "unit": "ns/iter",
            "extra": "iterations: 17255\ncpu: 40656.33474355349 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 317431.0135751855,
            "unit": "ns/iter",
            "extra": "iterations: 2214\ncpu: 317337.6553749224 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 630897.1951331173,
            "unit": "ns/iter",
            "extra": "iterations: 1107\ncpu: 630816.1743450814 ns\nthreads: 1"
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
          "id": "af7659367d7e887ca35576bf5d12619158cf1a7c",
          "message": "Add headless Lua API smoke test\n\nBuild a test that constructs a real Game (its constructor is SDL-free)\nand replays Game::Setup's binding sequence without opening a window, then\nasserts the scripting surface is fully wired: every registered component\nhas a usertype plus registry.has_/get_ accessors, one sentinel global per\nfree-function module resolves, and the primitive/input surfaces install.\nCatches the \"wrote a binding/module but never wired it\" class of bug at\nbuild/CI time instead of by eyeballing the example project's log.\n\nGated behind OCTARINE_ENABLE_TESTS (OFF by default); registers an\nOctarineLuaApiTest target + ctest entry. The test exe links the whole\nengine minus Main.cpp. The smoke artifact it writes (lua_api.smoke.lua)\nis gitignored.",
          "timestamp": "2026-05-23T11:54:58-06:00",
          "tree_id": "da4f068a1d927499e8475478344621dc1b888207",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/af7659367d7e887ca35576bf5d12619158cf1a7c"
        },
        "date": 1779559317114,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3990.4908532981417,
            "unit": "ns/iter",
            "extra": "iterations: 178023\ncpu: 4022.8128556420884 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15731.431966024344,
            "unit": "ns/iter",
            "extra": "iterations: 43562\ncpu: 15774.479316836136 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106323.7013045895,
            "unit": "ns/iter",
            "extra": "iterations: 6535\ncpu: 106370.509410866 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 902150.1484851858,
            "unit": "ns/iter",
            "extra": "iterations: 779\ncpu: 902223.513478829 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1832858.540486389,
            "unit": "ns/iter",
            "extra": "iterations: 384\ncpu: 1832870.567708346 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2354.664780887748,
            "unit": "ns/iter",
            "extra": "iterations: 307866\ncpu: 2352.319944391825 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9857.966025110301,
            "unit": "ns/iter",
            "extra": "iterations: 71233\ncpu: 9849.581345725835 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 100260.30233716975,
            "unit": "ns/iter",
            "extra": "iterations: 9819\ncpu: 100250.26357061433 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 393083.79696977953,
            "unit": "ns/iter",
            "extra": "iterations: 1788\ncpu: 393069.0262863351 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2140.5804920354553,
            "unit": "ns/iter",
            "extra": "iterations: 331859\ncpu: 2108.4110992897945 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6469.484986169422,
            "unit": "ns/iter",
            "extra": "iterations: 109178\ncpu: 6433.399008946939 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41068.474337959306,
            "unit": "ns/iter",
            "extra": "iterations: 17053\ncpu: 41033.658241945726 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 314674.350978474,
            "unit": "ns/iter",
            "extra": "iterations: 2201\ncpu: 314594.87687416427 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 630852.2762096338,
            "unit": "ns/iter",
            "extra": "iterations: 1104\ncpu: 630761.1775362464 ns\nthreads: 1"
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
          "distinct": false,
          "id": "07239c1f04594d0899981abaa23ff35749a0f1b1",
          "message": "Add a persistent scene control UI to the top of the editor and fixed sound control.",
          "timestamp": "2026-05-23T12:51:05-06:00",
          "tree_id": "e4d2fa529c0958f8876992dd7bb0e6c0a4018752",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/07239c1f04594d0899981abaa23ff35749a0f1b1"
        },
        "date": 1779562684403,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3910.110141506177,
            "unit": "ns/iter",
            "extra": "iterations: 179808\ncpu: 3935.0033813846317 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15934.420993598109,
            "unit": "ns/iter",
            "extra": "iterations: 43465\ncpu: 15969.258529852132 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107400.32027955778,
            "unit": "ns/iter",
            "extra": "iterations: 6483\ncpu: 107432.27826623552 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 902044.2767970024,
            "unit": "ns/iter",
            "extra": "iterations: 775\ncpu: 902041.4941935346 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1843576.915414714,
            "unit": "ns/iter",
            "extra": "iterations: 379\ncpu: 1843355.5013192967 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2186.6315023278385,
            "unit": "ns/iter",
            "extra": "iterations: 309103\ncpu: 2176.088540066185 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9981.984285116281,
            "unit": "ns/iter",
            "extra": "iterations: 69940\ncpu: 9971.41695739751 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 69420.3697005261,
            "unit": "ns/iter",
            "extra": "iterations: 9065\ncpu: 69406.3817981191 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 393955.41067984106,
            "unit": "ns/iter",
            "extra": "iterations: 1777\ncpu: 393916.2172200195 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2263.97519181015,
            "unit": "ns/iter",
            "extra": "iterations: 314579\ncpu: 2232.7820769973887 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6604.527772224941,
            "unit": "ns/iter",
            "extra": "iterations: 106843\ncpu: 6569.9843883024705 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41210.05973036775,
            "unit": "ns/iter",
            "extra": "iterations: 16814\ncpu: 41162.84084693286 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 317662.50235269393,
            "unit": "ns/iter",
            "extra": "iterations: 2212\ncpu: 317511.1993671483 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 631737.2651719445,
            "unit": "ns/iter",
            "extra": "iterations: 1090\ncpu: 631640.1449541847 ns\nthreads: 1"
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
          "id": "e8d4bfc456fe819e3f1cf58c52dfcbe42efba585",
          "message": "Add isFixed flag for screen-fixed colliders and UI buttons\n\nAdds a bool isFixed to BoxColliderComponent and UIButtonComponent\n(exposed to Lua as is_fixed, editable in the inspectors). When set, the\ncollider/button is treated as screen-space: DrawColliderSystem and the\nUIButtonSystem click test skip the camera offset so fixed HUD elements\nstay put regardless of camera scroll. Also exposes get_camera_position()\nto Lua for scripts that need the camera origin.",
          "timestamp": "2026-05-23T20:37:55-06:00",
          "tree_id": "e1a8b93476cfb5037a36886006d9c91bf083b8bc",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/e8d4bfc456fe819e3f1cf58c52dfcbe42efba585"
        },
        "date": 1779591286537,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3879.565168222605,
            "unit": "ns/iter",
            "extra": "iterations: 180790\ncpu: 3905.493871342322 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16039.90931443984,
            "unit": "ns/iter",
            "extra": "iterations: 43879\ncpu: 16072.704847422046 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106509.37874234011,
            "unit": "ns/iter",
            "extra": "iterations: 6501\ncpu: 106555.13505614351 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 906443.6908280525,
            "unit": "ns/iter",
            "extra": "iterations: 772\ncpu: 904976.358808299 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1823493.6212519964,
            "unit": "ns/iter",
            "extra": "iterations: 372\ncpu: 1823612.2983871025 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2189.151543019361,
            "unit": "ns/iter",
            "extra": "iterations: 312657\ncpu: 2174.4092280051204 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9747.447707055067,
            "unit": "ns/iter",
            "extra": "iterations: 72218\ncpu: 9732.847267994553 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 76051.66704321565,
            "unit": "ns/iter",
            "extra": "iterations: 9975\ncpu: 76029.01654135589 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 390899.46756571834,
            "unit": "ns/iter",
            "extra": "iterations: 1766\ncpu: 390882.3805209507 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2316.6543040276256,
            "unit": "ns/iter",
            "extra": "iterations: 307283\ncpu: 2280.2427664404877 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6627.981960464705,
            "unit": "ns/iter",
            "extra": "iterations: 105691\ncpu: 6589.1251856930685 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41045.50274007068,
            "unit": "ns/iter",
            "extra": "iterations: 17146\ncpu: 41001.721742693 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316898.4440437901,
            "unit": "ns/iter",
            "extra": "iterations: 2228\ncpu: 316723.67684021493 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 652800.9808607406,
            "unit": "ns/iter",
            "extra": "iterations: 1063\ncpu: 652642.7911571583 ns\nthreads: 1"
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
          "id": "bd33792f29fa501ff245df137522e9c104ef9145",
          "message": "Set option to output input events in debug logs",
          "timestamp": "2026-05-23T20:56:42-06:00",
          "tree_id": "2b44f6a00ce23a66e9f8f53fd276447636b0096d",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/bd33792f29fa501ff245df137522e9c104ef9145"
        },
        "date": 1779591798681,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3890.2990340587476,
            "unit": "ns/iter",
            "extra": "iterations: 177904\ncpu: 3924.1996863478184 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15943.381988885452,
            "unit": "ns/iter",
            "extra": "iterations: 44611\ncpu: 15981.731859853948 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 108035.37098399378,
            "unit": "ns/iter",
            "extra": "iterations: 6454\ncpu: 108089.7663464505 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 903290.4922258577,
            "unit": "ns/iter",
            "extra": "iterations: 773\ncpu: 903391.8538162968 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1825718.165377724,
            "unit": "ns/iter",
            "extra": "iterations: 383\ncpu: 1825784.1462140798 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2353.838687813191,
            "unit": "ns/iter",
            "extra": "iterations: 303229\ncpu: 2339.6953391666125 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9983.457720591638,
            "unit": "ns/iter",
            "extra": "iterations: 70239\ncpu: 9968.665342617049 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 68284.26329821307,
            "unit": "ns/iter",
            "extra": "iterations: 10214\ncpu: 68243.14558448468 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 389729.77747409494,
            "unit": "ns/iter",
            "extra": "iterations: 1801\ncpu: 389704.6007773402 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2107.0013532326334,
            "unit": "ns/iter",
            "extra": "iterations: 336000\ncpu: 2063.6415833343594 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6480.723409623933,
            "unit": "ns/iter",
            "extra": "iterations: 109360\ncpu: 6431.973555226865 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41198.747027547884,
            "unit": "ns/iter",
            "extra": "iterations: 16963\ncpu: 41143.261392431334 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 317362.4724994441,
            "unit": "ns/iter",
            "extra": "iterations: 2205\ncpu: 317278.4911564585 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 631633.2775230991,
            "unit": "ns/iter",
            "extra": "iterations: 1111\ncpu: 631501.5634563 ns\nthreads: 1"
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
          "id": "3934f08fcb6dde21cc8857f8dd4b24b3ba58d029",
          "message": "Fix some visual quirks with the lua console",
          "timestamp": "2026-05-23T21:04:46-06:00",
          "tree_id": "f92c51188047c2f3c1895cfd0f3c4aa1c8c92fcf",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/3934f08fcb6dde21cc8857f8dd4b24b3ba58d029"
        },
        "date": 1779592269390,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3880.3665737665738,
            "unit": "ns/iter",
            "extra": "iterations: 177569\ncpu: 3917.7145278734797 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15826.179973879223,
            "unit": "ns/iter",
            "extra": "iterations: 42297\ncpu: 15868.417429131385 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106809.79208915697,
            "unit": "ns/iter",
            "extra": "iterations: 6579\ncpu: 106506.2305821576 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 910278.3384074637,
            "unit": "ns/iter",
            "extra": "iterations: 764\ncpu: 910336.2133507815 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1836614.3634346754,
            "unit": "ns/iter",
            "extra": "iterations: 380\ncpu: 1836770.4342105107 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2216.570577075682,
            "unit": "ns/iter",
            "extra": "iterations: 294117\ncpu: 2209.032918873309 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9753.983484840013,
            "unit": "ns/iter",
            "extra": "iterations: 72218\ncpu: 9743.580533935932 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 68169.71093897367,
            "unit": "ns/iter",
            "extra": "iterations: 10220\ncpu: 68133.49755380725 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 390649.62736997043,
            "unit": "ns/iter",
            "extra": "iterations: 1798\ncpu: 390650.5678531886 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2148.32452700957,
            "unit": "ns/iter",
            "extra": "iterations: 331520\ncpu: 2109.4945945956033 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6491.664836389201,
            "unit": "ns/iter",
            "extra": "iterations: 108950\ncpu: 6449.288031201822 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40892.837025120716,
            "unit": "ns/iter",
            "extra": "iterations: 17120\ncpu: 40847.21699765249 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316487.88457297214,
            "unit": "ns/iter",
            "extra": "iterations: 2214\ncpu: 316353.0957542805 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 631723.6879714298,
            "unit": "ns/iter",
            "extra": "iterations: 1107\ncpu: 631631.1806684649 ns\nthreads: 1"
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
          "id": "406d4f7fca47faef912105f5319294bbc6eb527d",
          "message": "Fix windows build",
          "timestamp": "2026-05-23T21:08:13-06:00",
          "tree_id": "0d8ec41fe0627a507b4d000db7759b4e47cc1f3d",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/406d4f7fca47faef912105f5319294bbc6eb527d"
        },
        "date": 1779592709314,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3920.0412294696894,
            "unit": "ns/iter",
            "extra": "iterations: 177874\ncpu: 3947.5452005351085 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15800.43512901952,
            "unit": "ns/iter",
            "extra": "iterations: 44147\ncpu: 15835.315649987835 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 107862.00018111418,
            "unit": "ns/iter",
            "extra": "iterations: 6505\ncpu: 107909.85242121186 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 913221.5588846627,
            "unit": "ns/iter",
            "extra": "iterations: 765\ncpu: 913150.6653594449 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1844907.1609962082,
            "unit": "ns/iter",
            "extra": "iterations: 378\ncpu: 1844802.4603174732 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2339.6275961399597,
            "unit": "ns/iter",
            "extra": "iterations: 298431\ncpu: 2338.2048547233867 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 9919.315903507433,
            "unit": "ns/iter",
            "extra": "iterations: 70893\ncpu: 9911.207904871226 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 99684.33104030363,
            "unit": "ns/iter",
            "extra": "iterations: 8325\ncpu: 99662.6160960923 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 391475.90503313387,
            "unit": "ns/iter",
            "extra": "iterations: 1787\ncpu: 391449.4157806526 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2281.352991380523,
            "unit": "ns/iter",
            "extra": "iterations: 307844\ncpu: 2247.982471641027 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6627.968049763507,
            "unit": "ns/iter",
            "extra": "iterations: 106259\ncpu: 6588.705483767018 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40995.30906993206,
            "unit": "ns/iter",
            "extra": "iterations: 17114\ncpu: 40953.453020920366 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316406.5835690525,
            "unit": "ns/iter",
            "extra": "iterations: 2215\ncpu: 316317.9638825991 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 634951.3281929007,
            "unit": "ns/iter",
            "extra": "iterations: 1105\ncpu: 634786.900452519 ns\nthreads: 1"
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8c2f341b09f8d8b19cc5625706dff36400817ad7",
          "message": "Merge pull request #31 from mblackman/asset-pipeline\n\nAsset pipeline + Android build target",
          "timestamp": "2026-05-30T08:23:59-06:00",
          "tree_id": "190988cd7ba578411a87796dbf55216537e9d97a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/8c2f341b09f8d8b19cc5625706dff36400817ad7"
        },
        "date": 1780151443510,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 4050.7028248350275,
            "unit": "ns/iter",
            "extra": "iterations: 171932\ncpu: 4075.56388572188 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 16160.817593607278,
            "unit": "ns/iter",
            "extra": "iterations: 43411\ncpu: 16199.763447052466 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 106805.95952342049,
            "unit": "ns/iter",
            "extra": "iterations: 6591\ncpu: 106852.84949173202 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 847713.5742245947,
            "unit": "ns/iter",
            "extra": "iterations: 824\ncpu: 847870.2390776728 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1671786.5527773413,
            "unit": "ns/iter",
            "extra": "iterations: 421\ncpu: 1671924.0783847987 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/8",
            "value": 2301.0628653227673,
            "unit": "ns/iter",
            "extra": "iterations: 303882\ncpu: 2288.985905713764 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/64",
            "value": 10001.899319060369,
            "unit": "ns/iter",
            "extra": "iterations: 69287\ncpu: 9979.809083952923 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/512",
            "value": 100689.95966080311,
            "unit": "ns/iter",
            "extra": "iterations: 6923\ncpu: 100671.00476672933 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlamWithPairs/2048",
            "value": 397318.21314666,
            "unit": "ns/iter",
            "extra": "iterations: 1777\ncpu: 397275.17895329715 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2192.8911169094263,
            "unit": "ns/iter",
            "extra": "iterations: 326102\ncpu: 2156.4803619724544 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6602.994259986877,
            "unit": "ns/iter",
            "extra": "iterations: 107616\ncpu: 6564.307733054549 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 41541.78204902452,
            "unit": "ns/iter",
            "extra": "iterations: 16959\ncpu: 41490.745975596015 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 318882.8643607466,
            "unit": "ns/iter",
            "extra": "iterations: 2204\ncpu: 318743.44010891014 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 636732.5819893808,
            "unit": "ns/iter",
            "extra": "iterations: 1104\ncpu: 636629.4927535824 ns\nthreads: 1"
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
          "id": "a1fa1c3a9c5bb8c5fa88fda2efecef41dc793e35",
          "message": "Added render culling to render systems to prevent excessive rendering",
          "timestamp": "2026-05-14T23:10:40-06:00",
          "tree_id": "89bf99872f5e9ad4eae3beb2dc6d2caf62bb4236",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a1fa1c3a9c5bb8c5fa88fda2efecef41dc793e35"
        },
        "date": 1778822310493,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 2.982,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.326,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.372,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.356,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.684,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.974,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.534,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.433,
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
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.083,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 665"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 665"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 665"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 691"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 691"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 691"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.306,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.401,
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
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.155,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 737"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.174,
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
            "value": 0.009,
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
            "name": "Sweep Bipartite [p95]",
            "value": 6.335,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 6.748,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 6.954,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 4.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.134,
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
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.012,
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
            "value": 0.83,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.912,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 1.196,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 742"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.28,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.359,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.655,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.444,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.608,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 11.25,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.097,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.359,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.113,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.148,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.37,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.483,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.55,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.621,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.195,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.282,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.887,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.69,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.812,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.29,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.694,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 389"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 1.817,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 389"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.293,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 389"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.168,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.22,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.239,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.203,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.63,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 16.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.209,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.638,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 16.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.111,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.195,
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
          "id": "45644ec6adfd8b1dfa3355a35eedc03ba8c31801",
          "message": "Standardize naming of entity masks and allow assigning at entity or component level.\nAdded validation to prevent trying to set the entity mask twice.",
          "timestamp": "2026-05-15T09:04:22-06:00",
          "tree_id": "48dfff080b19dc0bbe58f2a8b62dca2d2fba2518",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/45644ec6adfd8b1dfa3355a35eedc03ba8c31801"
        },
        "date": 1778857947688,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Draw [p95]",
            "value": 2.741,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.297,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.415,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.363,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.43,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.312,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.854,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 673"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 673"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 673"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 686"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 686"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 686"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.281,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.318,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.351,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
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
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
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
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 1.1,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.165,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.998,
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
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 741"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.104,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 6.627,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.376,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 5.503,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
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
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.021,
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
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.742,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.829,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.931,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.073,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.088,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 740"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.255,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.304,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.365,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.438,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.605,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.636,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.109,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.148,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 1.25,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.335,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.888,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.31,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.542,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [max]",
            "value": 1.98,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.309,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 1.547,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 1.984,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.16,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.195,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 3.932,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.243,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.692,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 3.937,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.697,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.158,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.358,
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
          "id": "7d039d6dde106a8b38db88a560243d6fdc0133e1",
          "message": "Fix char",
          "timestamp": "2026-05-15T09:05:28-06:00",
          "tree_id": "7afde9a120a11510310242c71139c5692401238d",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/7d039d6dde106a8b38db88a560243d6fdc0133e1"
        },
        "date": 1778858421408,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.369,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.625,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.191,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
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
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.575,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.895,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.148,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.158,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.119,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.158,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.171,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.162,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.175,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.635,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.684,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.19,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.689,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 396"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 396"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 396"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 388"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 388"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 388"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.119,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.149,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.56,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.953,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.278,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.564,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.957,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.283,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.375,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.378,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.791,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.277,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 14.815,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.019,
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
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "d8196fb575d11613beb2d4acb1007c63c0e974d0",
          "message": "Added a name component that shows up in the hierarchy browser",
          "timestamp": "2026-05-15T09:15:17-06:00",
          "tree_id": "430160cbc73d5c54bb97d93460cf81fd313bf987",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/d8196fb575d11613beb2d4acb1007c63c0e974d0"
        },
        "date": 1778858899039,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.33,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.354,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.43,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.8,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.503,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 9.714,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.375,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.533,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 744"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.888,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.151,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.153,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.163,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.158,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.171,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.243,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.133,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.181,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.293,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.186,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.311,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 397"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 397"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 397"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 392"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 392"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 392"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.535,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.79,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.32,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.543,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.794,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.323,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 37"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 37"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 37"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "e531de6eabb08bbacce18aec7cbbe7ea860a6710",
          "message": "Added naming support to projectiles",
          "timestamp": "2026-05-15T09:23:05-06:00",
          "tree_id": "36a42fa199b223ea998697d9b0b109d5964d3d2c",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/e531de6eabb08bbacce18aec7cbbe7ea860a6710"
        },
        "date": 1778859373146,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.473,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.834,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 746"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 746"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 746"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.323,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.787,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.14,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.151,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.162,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.167,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.493,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.168,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.579,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.174,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.586,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 411"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 411"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.051,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 411"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 388"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 388"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 388"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 3.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.526,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.871,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.532,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.878,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.332,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.367,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.766,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.909,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.751,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 9.753,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 37"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 37"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 37"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "a9b5acbfb22c72cfdb7872c2a12d52641fbd466b",
          "message": "Add audio source/sink, pool & deferred commands\n\nIntroduce AudioSourceComponent and AudioSinkComponent and wire audio into the ECS/Lua stack. Revamp AudioSystem to use a managed track pool with generation-tagged slots, dynamic initial/max sizing from EngineOptions, safer acquisition/ownership checks, proper mixer lifecycle, and per-entity sink management (playOnSpawn, loop, despawnOnFinish). Add deferred function channel and templated AddComponent to CommandBuffer, and ensure system wrappers playback command buffers after system execution. Update Registry playback to handle deferred component adds. Expose audio_source creation in the Lua component factory and register the AudioSystem via the Registry in Game::Setup; Init now reports failure instead of assuming success. These changes enable safe, deferred component mutations from systems, reduce per-frame audio retries, and prevent pool reuse races.",
          "timestamp": "2026-05-15T09:46:43-06:00",
          "tree_id": "7dd89baef189dfdd9ad8f46d953957278095d914",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a9b5acbfb22c72cfdb7872c2a12d52641fbd466b"
        },
        "date": 1778860474067,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.325,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.354,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 4.896,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.844,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.905,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 15.962,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.426,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.668,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.204,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.148,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.151,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.161,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.34,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.17,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.392,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.175,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 390"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 390"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 390"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 382"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.099,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.517,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.728,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.933,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.521,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.732,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.937,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.032,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.111,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 22"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 22"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 22"
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
          "id": "a8984cf57607ecab7b050ee486761c02a88e0c92",
          "message": "Strip out unused code and optimize LTO for dependencies in release builds for smaller executables",
          "timestamp": "2026-05-16T11:07:52-06:00",
          "tree_id": "e6b6f981501c2018eb3a64bbd6b9507af190c981",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a8984cf57607ecab7b050ee486761c02a88e0c92"
        },
        "date": 1778951747401,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.378,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.532,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1110"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1110"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1110"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.118,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.375,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.764,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.146,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.118,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.145,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.152,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.16,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.327,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.159,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.375,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.164,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.379,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 387"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 387"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 387"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 383"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 383"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 383"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.1,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.291,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.572,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.81,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.166,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.578,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.812,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.171,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.332,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.378,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.421,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.737,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 9.589,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 10.825,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "7a2184f940e12d81919a9f198189d13e0f7cad3c",
          "message": "Specify media types to reduce build size a smidge",
          "timestamp": "2026-05-16T11:15:30-06:00",
          "tree_id": "343754e5eb93492bfcb4101117b1a5638865b31a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/7a2184f940e12d81919a9f198189d13e0f7cad3c"
        },
        "date": 1778952191478,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.327,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.349,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.46,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.804,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.778,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 10.882,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.579,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.539,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.705,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.141,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.147,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.161,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.151,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.166,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.161,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.35,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.213,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.409,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.218,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.412,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 402"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 402"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 402"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.141,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.286,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.551,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.879,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.555,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.883,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.09,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.084,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 25"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 25"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 25"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "a515d347952869b4123da412ac9f01519a36c6bf",
          "message": "Fix macos build error",
          "timestamp": "2026-05-16T11:16:33-06:00",
          "tree_id": "f6bba67fed2cfe01e52fde5c8becbbf63b9841d2",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/a515d347952869b4123da412ac9f01519a36c6bf"
        },
        "date": 1778952629164,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.331,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.348,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.363,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.834,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.801,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 10.336,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.269,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.397,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.151,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.628,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.755,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.149,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.154,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.157,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.215,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.278,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.141,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.257,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.349,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.146,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.261,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.354,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 378"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 398"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 398"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.084,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 398"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.102,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 3.505,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.623,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 1.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 4.149,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.627,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 1.049,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 4.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 24"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 24"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 24"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "1c189fe8b8fba5764ce7641a56eff89925e5a9e9",
          "message": "Move all build artifacts to build/ directory",
          "timestamp": "2026-05-16T11:47:23-06:00",
          "tree_id": "99acc612eafe6d5e790615e0e47ae9b28d002706",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/1c189fe8b8fba5764ce7641a56eff89925e5a9e9"
        },
        "date": 1778954075542,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.321,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.341,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.385,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 7.979,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 9.783,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.271,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.499,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.237,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.567,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.666,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.166,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.173,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.162,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.179,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.216,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.367,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.133,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.269,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.415,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.273,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.422,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 393"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 393"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.048,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 393"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 397"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 397"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 397"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.693,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.907,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.696,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.91,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 35"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 35"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 35"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "9bb7838427c2744671ed0b1e3121218228538d10",
          "message": "Break up TransformComponent.h into multiple parts.\nThis change breaks apart the different fields of a TransformComponent.h into different components to reduce unused fields for such a common component.\nUpdated all systems and Lua loading.",
          "timestamp": "2026-05-16T15:57:20-06:00",
          "tree_id": "0e0f2d582d38490fb6ff3bb045cea68837f8e2b4",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/9bb7838427c2744671ed0b1e3121218228538d10"
        },
        "date": 1778969076217,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.34,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.388,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.441,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.857,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.508,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.677,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.017,
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
            "name": "Game::WaitTime [p95]",
            "value": 9.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.084,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.383,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.654,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild) [p95]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild) [p99]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild) [max]",
            "value": 0.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (walk) [p95]",
            "value": 0.215,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (walk) [p99]",
            "value": 0.231,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (walk) [max]",
            "value": 0.265,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.25,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.281,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.449,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.256,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.287,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.453,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.1,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.251,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.259,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 379"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 403"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 403"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 403"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 380"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 380"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 380"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.091,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.655,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.857,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.456,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.658,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.861,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.46,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 29"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 29"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 29"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "d1929920d4a7af05f6d22a253cd0de3bc97fa377",
          "message": "Added support for optional components in queries.\nFixed some issues with copying components in queries that caused unneeded copying.",
          "timestamp": "2026-05-16T19:04:49-06:00",
          "tree_id": "f0c17789c8932d3c62b4b83374341bfd68294112",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/d1929920d4a7af05f6d22a253cd0de3bc97fa377"
        },
        "date": 1778980326863,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.321,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.345,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.36,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.572,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.095,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 11.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.36,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.699,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.091,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.631,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p95]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p99]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.053,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.051,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.499,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.133,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.201,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.561,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.138,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.212,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.564,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 376"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 403"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 403"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 403"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.141,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.511,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.764,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.961,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.515,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.773,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.964,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "2be94b328fc19520a5e7560703c88ddd4a9aebdc",
          "message": "Lua-driven InputSystem; split MovementSystem into focused systems; drop KeyboardControlSystem\n\nAdds InputSystem (header-only, owned by Registry) that exposes a global `input` table\nto Lua: polling (is_key_down/pressed/released, is_mouse_*, mouse_position, mouse_wheel),\naction bindings (bind/unbind, is_action_down/pressed/released), and callbacks\n(on_key_down/up, on_mouse_down/up, on_mouse_wheel). Subscribes to KeyInputEvent,\nMouseInputEvent, and a new MouseWheelEvent emitted from Game::ProcessInput.\n\nMovementSystem is split by responsibility: VelocityIntegrationSystem (position += velocity*dt),\nOffScreenDespawnSystem (despawns non-player entities outside the playable area), and\nObstacleBounceSystem (event-driven enemy<->obstacle reflect). KeyboardControlSystem,\nMovementSystem, and KeyboardControlComponent are removed; player movement and clamping\nmove to Lua via input.is_action_* + get/set_position + get_game_map_dimensions.\n\nProjectileEmitSystem loses its per-frame system pass and the SPACE auto-fire hook; it\nnow owns only the projectile pool and a public Fire(registry, entity, direction).\nGameplay drives shots via a new fire_projectile(entity, dx, dy) Lua binding. Auto-fire\nfor enemies moves to Lua scripts (see example project's lib/auto_fire.lua).\n\nScriptSystem is trimmed to ScriptComponent dispatch; key polling state and bindings\nmove to InputSystem. Adds get_game_map_dimensions() getter. InputSystem.ResetLuaState\nruns on StopScene/LoadScene to drop stale callbacks/actions.\n\nDocs: removes keyboard_controller component reference; rewrites the player example to\nuse the input table; adds a full input-table API reference.",
          "timestamp": "2026-05-16T21:47:48-06:00",
          "tree_id": "f95786a0bcb065467239575b4859f512622e4672",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2be94b328fc19520a5e7560703c88ddd4a9aebdc"
        },
        "date": 1778990229770,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.331,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.032,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.287,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.293,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.809,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.141,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.3,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p95]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p99]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [max]",
            "value": 0.268,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.048,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.282,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.289,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.853,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.116,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.867,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 385"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 405"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 405"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.089,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 405"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.049,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 391"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.204,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.52,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.755,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.968,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.525,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.764,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.979,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.344,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.366,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.395,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.913,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.464,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 9.221,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "7f834012b877105d2054600ee8b3474934bf88d1",
          "message": "Remove display health system and add related components to Lua support",
          "timestamp": "2026-05-17T11:09:59-06:00",
          "tree_id": "b76bb375a84cddc448c1732c9096237985ebccb9",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/7f834012b877105d2054600ee8b3474934bf88d1"
        },
        "date": 1779038249603,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.332,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.363,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.938,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.679,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 9.826,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.182,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.289,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1107"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1107"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1107"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.189,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.533,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.736,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.313,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.579,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p95]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p99]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.213,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.05,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.217,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.083,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.266,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.121,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.329,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.141,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.333,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 395"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 395"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 395"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.119,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.133,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.677,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.987,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.179,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.681,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.991,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.185,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.09,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 22"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 22"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 22"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "52df981b386d9959be4c9c4fbdb8d595756bf2c4",
          "message": "Add new Lua Binding abstraction to make adding new bindings straightforward. Migrate existing bindings.",
          "timestamp": "2026-05-17T11:36:16-06:00",
          "tree_id": "d6d15b2c693fdd02db318ff34d37ce1f3fd944fc",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/52df981b386d9959be4c9c4fbdb8d595756bf2c4"
        },
        "date": 1779039821750,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.328,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.351,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.358,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.628,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 8.915,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.016,
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
            "name": "Game::WaitTime [p95]",
            "value": 9.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.296,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.46,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.032,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.046,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.345,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.635,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.128,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.158,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.177,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p95]",
            "value": 0.036,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p99]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [max]",
            "value": 0.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.048,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.356,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.167,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.416,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.172,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.421,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 375"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 394"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 394"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 394"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.595,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.799,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.934,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.6,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.802,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.938,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.053,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 20"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
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
          "id": "6f35a51f22747490b8f62972aad577297a289de4",
          "message": "Add rotation/scale Lua bindings and wire rotation through rendering and collision\n\nExpose PositionComponent and RotationComponent via LuaComponentRegistry\nso registry.has_*/get_* and the entity-table component keys work for the\nfull local transform triplet.\n\nPipe GlobalTransformComponent.rotation through SquareCommand and render\nrotated quads via SDL_RenderGeometry. Draw rotated box colliders as a\nwireframe OBB. Promote CollisionSystem boxes to oriented bounding boxes:\nbroadphase still uses an AABB enclosing the OBB so the median-cut sweep\nis unchanged; narrowphase adds SAT over the four face normals when at\nleast one box is rotated.",
          "timestamp": "2026-05-18T22:08:38-06:00",
          "tree_id": "f8e7d49ddd9a31fc1af58cb7eb79eb9602b79c4e",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/6f35a51f22747490b8f62972aad577297a289de4"
        },
        "date": 1779164278738,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.647,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1110"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1110"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1110"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.016,
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
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.028,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.05,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.127,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.121,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.482,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.667,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.684,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 1.08,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.327,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.355,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 369"
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
          "id": "2c630ba63d71586571fed8b4900ab392558241bc",
          "message": "Add declarative Lua free-function modules; add HealthComponent methods\n\nMirror the LuaBinding<T> component pattern for global free functions.\nEach domain gets a LuaModuleBinding<M> specialization owning its\ninstall(sol::state&, Game&); RegisterAllModules.cpp drives them in one\nlist. Migrates the hand-wired blocks out of ScriptSystem: globals split\ninto Log/Io/Assets/Audio/Entity/Scene/Game modules, EntityModule owns\nthe registry table + per-component accessor loop. Global names unchanged\nso existing scripts keep working.\n\nScriptSystem::CreateLuaBindings now only binds primitive + component\nusertypes and drops its Game param. Game::Setup calls CreateLuaBindings\n-> RegisterAllLuaModules -> bindAll. ProjectileEmitSystem (and\nEntityPoolManager) Set+Init early so GameModule binds fire_projectile\nagainst the live instance during module install.\n\nHealthComponent gains damage/heal/isDead/fraction. current_health binds\nas a clamping sol::property so Lua field writes cannot bypass invariants;\nis_dead/fraction expose as read-only properties. LuaBinding.h documents\nthe methods-in-bindUsertype pattern and the own-fields-only rule.",
          "timestamp": "2026-05-23T11:31:11-06:00",
          "tree_id": "b12caa008feedef3e0c242dbee12e3b125ffadd2",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2c630ba63d71586571fed8b4900ab392558241bc"
        },
        "date": 1779558152150,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.453,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.671,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
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
            "value": 0.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.81,
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
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.115,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.264,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.084,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.122,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.276,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.427,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.665,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.837,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 1.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.318,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.765,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.038,
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
          "id": "af7659367d7e887ca35576bf5d12619158cf1a7c",
          "message": "Add headless Lua API smoke test\n\nBuild a test that constructs a real Game (its constructor is SDL-free)\nand replays Game::Setup's binding sequence without opening a window, then\nasserts the scripting surface is fully wired: every registered component\nhas a usertype plus registry.has_/get_ accessors, one sentinel global per\nfree-function module resolves, and the primitive/input surfaces install.\nCatches the \"wrote a binding/module but never wired it\" class of bug at\nbuild/CI time instead of by eyeballing the example project's log.\n\nGated behind OCTARINE_ENABLE_TESTS (OFF by default); registers an\nOctarineLuaApiTest target + ctest entry. The test exe links the whole\nengine minus Main.cpp. The smoke artifact it writes (lua_api.smoke.lua)\nis gitignored.",
          "timestamp": "2026-05-23T11:54:58-06:00",
          "tree_id": "da4f068a1d927499e8475478344621dc1b888207",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/af7659367d7e887ca35576bf5d12619158cf1a7c"
        },
        "date": 1779559366961,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.415,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 19.169,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
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
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.021,
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
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.377,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.653,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.856,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 0.892,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.27,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.805,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.023,
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
          "distinct": false,
          "id": "07239c1f04594d0899981abaa23ff35749a0f1b1",
          "message": "Add a persistent scene control UI to the top of the editor and fixed sound control.",
          "timestamp": "2026-05-23T12:51:05-06:00",
          "tree_id": "e4d2fa529c0958f8876992dd7bb0e6c0a4018752",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/07239c1f04594d0899981abaa23ff35749a0f1b1"
        },
        "date": 1779562734524,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.343,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.842,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
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
            "value": 0.032,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.014,
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
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.089,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.115,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.096,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.153,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.403,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.666,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.681,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 0.986,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.346,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.386,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.013,
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
            "value": 0.02,
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
          "id": "e8d4bfc456fe819e3f1cf58c52dfcbe42efba585",
          "message": "Add isFixed flag for screen-fixed colliders and UI buttons\n\nAdds a bool isFixed to BoxColliderComponent and UIButtonComponent\n(exposed to Lua as is_fixed, editable in the inspectors). When set, the\ncollider/button is treated as screen-space: DrawColliderSystem and the\nUIButtonSystem click test skip the camera offset so fixed HUD elements\nstay put regardless of camera scroll. Also exposes get_camera_position()\nto Lua for scripts that need the camera origin.",
          "timestamp": "2026-05-23T20:37:55-06:00",
          "tree_id": "e1a8b93476cfb5037a36886006d9c91bf083b8bc",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/e8d4bfc456fe819e3f1cf58c52dfcbe42efba585"
        },
        "date": 1779591336523,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.394,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 25.652,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
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
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.014,
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
            "value": 0.001,
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
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.113,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.093,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.472,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.654,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.665,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 1.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.304,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.353,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.024,
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
          "id": "bd33792f29fa501ff245df137522e9c104ef9145",
          "message": "Set option to output input events in debug logs",
          "timestamp": "2026-05-23T20:56:42-06:00",
          "tree_id": "2b44f6a00ce23a66e9f8f53fd276447636b0096d",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/bd33792f29fa501ff245df137522e9c104ef9145"
        },
        "date": 1779591849089,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.086,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.343,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.654,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.081,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.1,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.168,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.091,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.109,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.207,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.374,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.655,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.679,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 0.913,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.328,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.688,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 370"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.021,
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
          "id": "3934f08fcb6dde21cc8857f8dd4b24b3ba58d029",
          "message": "Fix some visual quirks with the lua console",
          "timestamp": "2026-05-23T21:04:46-06:00",
          "tree_id": "f92c51188047c2f3c1895cfd0f3c4aa1c8c92fcf",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/3934f08fcb6dde21cc8857f8dd4b24b3ba58d029"
        },
        "date": 1779592319448,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.274,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.415,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
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
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1119"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.017,
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
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.091,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.377,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.385,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.347,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.62,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.656,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 0.826,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.211,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.362,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 373"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.025,
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
          "id": "406d4f7fca47faef912105f5319294bbc6eb527d",
          "message": "Fix windows build",
          "timestamp": "2026-05-23T21:08:13-06:00",
          "tree_id": "0d8ec41fe0627a507b4d000db7759b4e47cc1f3d",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/406d4f7fca47faef912105f5319294bbc6eb527d"
        },
        "date": 1779592759389,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::WaitTime [p95]",
            "value": 16.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 16.331,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.525,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.004,
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
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1116"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.011,
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
            "value": 0.001,
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
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.023,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.087,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 0.113,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.097,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 0.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 0.161,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p95]",
            "value": 0.444,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.655,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.865,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 1.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 1.335,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 1.679,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.064,
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8c2f341b09f8d8b19cc5625706dff36400817ad7",
          "message": "Merge pull request #31 from mblackman/asset-pipeline\n\nAsset pipeline + Android build target",
          "timestamp": "2026-05-30T08:23:59-06:00",
          "tree_id": "190988cd7ba578411a87796dbf55216537e9d97a",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/8c2f341b09f8d8b19cc5625706dff36400817ad7"
        },
        "date": 1780151495292,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Present [p95]",
            "value": 0.327,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [p99]",
            "value": 0.352,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Render: Present [max]",
            "value": 0.546,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 7.752,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 8.241,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 8.968,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 372"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.031,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 9.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 9.315,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 9.686,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.574,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.744,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 1113"
          },
          {
            "name": "AudioSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AudioSystem [max]",
            "value": 0.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [p95]",
            "value": 0.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [p99]",
            "value": 0.44,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "VelocityIntegrationSystem [max]",
            "value": 0.577,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [p95]",
            "value": 0.154,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [p99]",
            "value": 0.177,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "OffScreenDespawnSystem [max]",
            "value": 0.315,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p95]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [p99]",
            "value": 0.05,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (roots rebuild + walk start) [max]",
            "value": 0.053,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow (descendants walk) [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p95]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [p99]",
            "value": 0.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem: Slow [max]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.057,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.085,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 0.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 0.123,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Collision System Update [max]",
            "value": 0.162,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 0.107,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 0.129,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 0.169,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 374"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 415"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 415"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 415"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.093,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 0.154,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.035,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.043,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 0.818,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 1.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 1.201,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 0.822,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 1.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 1.205,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Sort [max]",
            "value": 0.027,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 0.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 0.07,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Render: Draw [max]",
            "value": 0.089,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 371"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 36"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 5"
          }
        ]
      }
    ]
  }
}
window.BENCHMARK_DATA = {
  "lastUpdate": 1778560536709,
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
      }
    ]
  }
}
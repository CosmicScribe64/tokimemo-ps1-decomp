# T-24003 BUNKAKEN completion pass

Two accepted functions are in production source: func_80140F60 (296 bytes) and func_8013BD84 (540 bytes). Both pass literal default linked proof; BUNKAKEN SHA1 passes with both present. No header changes or fakematches were needed for those matches.

The remaining loader func_8013B510 stays INCLUDE_ASM. Its archived body is the T-14500 repairs/t15900 seed rewritten against current GameState fields and BUNKAKEN table types, with old-style main-function call views. The self-store retained from that seed is a candidate matching device, labelled FAKE; it has not been accepted into production. The helper func_8013BFA0 forward declaration is required because its void definition occurs later in the object. Put the MAIN_API_OVERRIDE guards from declarations.h before the includes when building this body in the full object.

Fresh aligned resolved full-TU scores (tools/order_search.make_text_scorer):

| Candidate | Cost |
| --- | ---: |
| typed arrays and GameState fields, no self-store | 335 |
| GameState local pointer | 335 |
| old-style func_800847B8 / func_80083440 | 335 |
| old-style calls and retained heroine self-store (archived body) | 318 |
| explicit local assigned at each heroine read | 318 |
| BunkaDispatch8 view | 318 |
| s32 function return view | 318 |
| dispatch plus s32 return view | 318 |
| fourth formal parameter for heroine (diagnostic only, discarded) | 326 |

The historical cost 297 predates current headers and is not a fresh result. The target retains the repeated D_80122EBC loads in a3; current C allocates them in v1. The source device that gives that shared allocation has not been recovered. There is also a temporary-numbering difference after the case-17 nibble load. Do not repeat the bounded variants above without new evidence. Experiment scripts, objects and instruction diagnostics remain ignored under build/t24003.

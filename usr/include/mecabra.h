/*
	mecabra.h
*/
#ifndef __MECABRA_H__
#define __MECABRA_H__

#include <CoreFoundation/CoreFoundation.h>

#ifdef __cplusplus
extern "C" {
#endif
    
// Temporarily make it public for the smooth transition to iPhone
struct mecabra_candidate_t {
    CFStringRef str;
    CFStringRef reading;
    uint8_t wordNum;
    uint8_t lastIsPrefix; // Now it has values 0..2. 0=input yomi is complete, 1=input yomi is incomplete for the last word. 2=input yomi is incomplete for the one before the last word.
    uint8_t kind;
    uint8_t kind_index;
    uint8_t wordLens[10];
    uint8_t readingLens[10];
    unsigned short lcAttrs[10];
    unsigned short rcAttrs[10];
    unsigned int trievalues[10];
    int weight;
};
typedef struct mecabra_candidate_t mecabra_candidate_t;
typedef struct mecabra_t mecabra_t;
    

/* ====================================
    Pre-defined keys for parameter options(CFDictionary) of mecabra_new()
   ==================================== */
/* 
    kMecabraOptionAmbiguousSearchKey
    If its value is TRUE, do abmiguous search. Otherwize, not.
    Key value: CFBoolean 
*/
extern const CFStringRef kMecabraOptionAmbiguousSearchKey;
/*  
    kMecabraOptionAddressBookNamePhoneticPairsKey
    Key value: CFArray of name-phonetic pair of AddressBook entries.
               Each name and each phonetic is CFString.
*/
extern const CFStringRef kMecabraOptionAddressBookNamePhoneticPairsKey;
/*
    kMecabraOptionUseIputStringAsTopCandidateKey
    if its value is TRUE, the input string will be the top candidate in candidate list.
    Key value: CFBoolean
*/
extern const CFStringRef kMecabraOptionUseIputStringAsTopCandidateKey;
/* 
    kMecabraOptionAdditionalDicPathsKey
    Key value: CFArray of additional dictionary CFURL 
*/
extern const CFStringRef kMecabraOptionAdditionalDicPathsKey;


/* ====================================
     Interfaces
   ==================================== */
/* 
/*
    Create a mecabra_t object
*/
mecabra_t* mecabra_new(CFURLRef system_dic_dir, CFURLRef learn_dic_dir, CFDictionaryRef options);

/*
    Analyze the string
*/
/* Analysis options */
enum {
	// not do prediction analysis
    kAnalysisOptionNoPrediction                    = 1UL << 0,
    kAnalysisOptionSingleWord                      = 1UL << 1
};
typedef unsigned long mecabra_analyze_options;

bool mecabra_analyze(mecabra_t* mecabra, CFStringRef string, CFRange range, mecabra_analyze_options options);

/*
    Iterate to get the next candidate
*/
mecabra_candidate_t* mecabra_next_candidate(mecabra_t* mecabra);

/*
    Get the surface string of mecabra_candidate_t
*/
CFStringRef mecabra_candidate_get_surface(mecabra_t* mecabra, mecabra_candidate_t *candidate);

/*
    Confirm a candidate so that mecabra can learn the candidate and predict the following candidates.
    mecabra_next_candidate() can be invoked to get the predicted candidates
*/
bool mecabra_confirm_candidate(mecabra_t* mecabra, mecabra_candidate_t* candidate);

/*
    Save learn dic
*/
void mecabra_save_learndic(mecabra_t* mecabra);
    
/*
    clear learn dic
*/
void mecabra_clear_learndic(mecabra_t* mecabra);
    
/*
    Returns an array of learning dictionary names. Used to delete learning dictionary files.
*/
CFArrayRef mecabra_create_learndic_names(void);

/*
	Dispose mecabra_t object
*/
void mecabra_destroy(mecabra_t* mecabra);

#ifdef __cplusplus
}
#endif

#endif /*__MECABRA_H__*/

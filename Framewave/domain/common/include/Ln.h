/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _LN_H_
#define _LN_H_

#include "fe.h"


//  TABLES for ln 8u, 16s, 32s, 64s, 32f, 64f


namespace OPT_LEVEL
{

namespace LN_TABLES {

    SYS_FORCEALIGN_16 static const float ln_8u_tbl[256] = 
    {
        0.00000F,  0.00000F,  0.69315F,  1.09861F,  1.38629F,  
        1.60944F,  1.79176F,  1.94591F,  2.07944F,  2.19722F,  
        2.30259F,  2.39790F,  2.48491F,  2.56495F,  2.63906F,  
        2.70805F,  2.77259F,  2.83321F,  2.89037F,  2.94444F,  
        2.99573F,  3.04452F,  3.09104F,  3.13549F,  3.17805F,  
        3.21888F,  3.25810F,  3.29584F,  3.33220F,  3.36730F,  
        3.40120F,  3.43399F,  3.46574F,  3.49651F,  3.52636F,  
        3.55535F,  3.58352F,  3.61092F,  3.63759F,  3.66356F,  
        3.68888F,  3.71357F,  3.73767F,  3.76120F,  3.78419F,  
        3.80666F,  3.82864F,  3.85015F,  3.87120F,  3.89182F,  
        3.91202F,  3.93183F,  3.95124F,  3.97029F,  3.98898F,  
        4.00733F,  4.02535F,  4.04305F,  4.06044F,  4.07754F,  
        4.09434F,  4.11087F,  4.12713F,  4.14313F,  4.15888F,  
        4.17439F,  4.18965F,  4.20469F,  4.21951F,  4.23411F,  
        4.24850F,  4.26268F,  4.27667F,  4.29046F,  4.30407F,  
        4.31749F,  4.33073F,  4.34381F,  4.35671F,  4.36945F,  
        4.38203F,  4.39445F,  4.40672F,  4.41884F,  4.43082F,  
        4.44265F,  4.45435F,  4.46591F,  4.47734F,  4.48864F,  
        4.49981F,  4.51086F,  4.52179F,  4.53260F,  4.54329F,  
        4.55388F,  4.56435F,  4.57471F,  4.58497F,  4.59512F,  
        4.60517F,  4.61512F,  4.62497F,  4.63473F,  4.64439F,  
        4.65396F,  4.66344F,  4.67283F,  4.68213F,  4.69135F,  
        4.70048F,  4.70953F,  4.71850F,  4.72739F,  4.73620F,  
        4.74493F,  4.75359F,  4.76217F,  4.77068F,  4.77912F,  
        4.78749F,  4.79579F,  4.80402F,  4.81218F,  4.82028F,  
        4.82831F,  4.83628F,  4.84419F,  4.85203F,  4.85981F,  
        4.86753F,  4.87520F,  4.88280F,  4.89035F,  4.89784F,  
        4.90527F,  4.91265F,  4.91998F,  4.92725F,  4.93447F,  
        4.94164F,  4.94876F,  4.95583F,  4.96284F,  4.96981F,  
        4.97673F,  4.98361F,  4.99043F,  4.99721F,  5.00395F,  
        5.01064F,  5.01728F,  5.02388F,  5.03044F,  5.03695F,  
        5.04343F,  5.04986F,  5.05625F,  5.06259F,  5.06890F,  
        5.07517F,  5.08140F,  5.08760F,  5.09375F,  5.09987F,  
        5.10595F,  5.11199F,  5.11799F,  5.12396F,  5.12990F,  
        5.13580F,  5.14166F,  5.14749F,  5.15329F,  5.15906F,  
        5.16479F,  5.17048F,  5.17615F,  5.18178F,  5.18739F,  
        5.19296F,  5.19850F,  5.20401F,  5.20949F,  5.21494F,  
        5.22036F,  5.22575F,  5.23111F,  5.23644F,  5.24175F,  
        5.24702F,  5.25227F,  5.25750F,  5.26269F,  5.26786F,  
        5.27300F,  5.27811F,  5.28320F,  5.28827F,  5.29330F,  
        5.29832F,  5.30330F,  5.30827F,  5.31321F,  5.31812F,  
        5.32301F,  5.32788F,  5.33272F,  5.33754F,  5.34233F,  
        5.34711F,  5.35186F,  5.35659F,  5.36129F,  5.36598F,  
        5.37064F,  5.37528F,  5.37990F,  5.38450F,  5.38907F,  
        5.39363F,  5.39816F,  5.40268F,  5.40717F,  5.41165F,  
        5.41610F,  5.42054F,  5.42495F,  5.42935F,  5.43372F,  
        5.43808F,  5.44242F,  5.44674F,  5.45104F,  5.45532F,  
        5.45959F,  5.46383F,  5.46806F,  5.47227F,  5.47646F,  
        5.48064F,  5.48480F,  5.48894F,  5.49306F,  5.49717F,  
        5.50126F,  5.50533F,  5.50939F,  5.51343F,  5.51745F,  
        5.52146F,  5.52545F,  5.52943F,  5.53339F,  5.53733F, 5.54126F
    };

//    Reference for all the tables below:
//    Table driven implementation of the Logarithm function in 
//    IEEE floating point arithmetic -- PING TAK PETER TANG 


    // sum of lead and trail tables used in ln_16s_ 
    SYS_FORCEALIGN_16 STATIC const float ln_16s_tbl[] = 
    {    
        0.0000000000F,  0.0077821403F,  0.0155041870F,  0.0231670588F,  0.0307716578F,  
        0.0383188650F,  0.0458095372F,  0.0532445163F,  0.0606246218F,  0.0679506585F,  
        0.0752234235F,  0.0824436694F,  0.0896121562F,  0.0967296287F,  0.1037967950F,  
        0.1108143628F,  0.1177830324F,  0.1247034818F,  0.1315763593F,  0.1384023279F,  
        0.1451820135F,  0.1519160420F,  0.1586050242F,  0.1652495712F,  0.1718502641F,  
        0.1784076542F,  0.1849223375F,  0.1913948506F,  0.1978257447F,  0.2042155415F,  
        0.2105647624F,  0.2168739438F,  0.2231435478F,  0.2293740958F,  0.2355660647F,  
        0.2417199314F,  0.2478361577F,  0.2539152205F,  0.2599575222F,  0.2659635544F,  
        0.2719337046F,  0.2778684497F,  0.2837681770F,  0.2896333039F,  0.2954642177F,  
        0.3012613356F,  0.3070250452F,  0.3127557039F,  0.3184537292F,  0.3241194785F,  
        0.3297532797F,  0.3353555501F,  0.3409265876F,  0.3464667797F,  0.3519764245F,  
        0.3574558794F,  0.3629055023F,  0.3683255613F,  0.3737164140F,  0.3790783584F,  
        0.3844116926F,  0.3897167444F,  0.3949938118F,  0.4002431631F,  0.4054650962F,  
        0.4106599391F,  0.4158279002F,  0.4209693074F,  0.4260843992F,  0.4311734736F,  
        0.4362367690F,  0.4412745535F,  0.4462870955F,  0.4512746334F,  0.4562374353F,  
        0.4611757100F,  0.4660897255F,  0.4709797204F,  0.4758459032F,  0.4806885421F,  
        0.4855078161F,  0.4903039932F,  0.4950772524F,  0.4998278618F,  0.5045560002F,  
        0.5092619061F,  0.5139457583F,  0.5186077356F,  0.5232481360F,  0.5278670788F,  
        0.5324648023F,  0.5370414853F,  0.5415973067F,  0.5461324453F,  0.5506471395F,  
        0.5551415086F,  0.5596157908F,  0.5640701652F,  0.5685047507F,  0.5729197264F,  
        0.5773153901F,  0.5816917419F,  0.5860490203F,  0.5903874636F,  0.5947071314F,  
        0.5990082026F,  0.6032908559F,  0.6075552702F,  0.6118015647F,  0.6160298586F,  
        0.6202403903F,  0.6244332790F,  0.6286086440F,  0.6327666640F,  0.6369074583F,  
        0.6410312057F,  0.6451379657F,  0.6492279172F,  0.6533012986F,  0.6573580503F,  
        0.6613984704F,  0.6654226184F,  0.6694306731F,  0.6734226942F,  0.6773988008F,  
        0.6813592315F,  0.6853039861F,  0.6892333031F,  0.6931471825F 
    };

    SYS_FORCEALIGN_16 STATIC const double ln_64f_lead_tbl[129] =
    {
    0                      ,  0.0077821404411224648356,  0.015504186534599284641,  0.023167059280240209773,
    0.030771658666708390228,  0.038318864300890709274 ,  0.045809536029992159456,  0.053244514518155483529,
    0.060624621815804857761,  0.067950661907161702402 ,  0.075223421235932619311,  0.082443669210988446139,
    0.089612158688396448269,  0.096729626457090489566 ,  0.10379679368088545743 ,  0.11081436633867269848 ,
    0.11778303565552050713 ,  0.12470347849921381567  ,  0.13157635778770782053 ,  0.13840232285838283133 ,
    0.14518200984275608789 ,  0.15191604202482267283  ,  0.15860503017574956175 ,  0.16524957289402664173 ,
    0.17185025692560884636 ,  0.1784076574713253649   ,  0.1849223384924698621  ,  0.191394852998200804   ,
    0.19782574332930380479 ,  0.20421554142740205862  ,  0.21056476910598576069 ,  0.21687393829961365554 ,
    0.22314355131311458535 ,  0.22937410106351308059  ,  0.23556607131104101427 ,  0.24171993688651127741 ,
    0.24783616390413953923 ,  0.25391520997982297558  ,  0.25995752443668607157 ,  0.26596354849607450888 ,
    0.27193371548310096841 ,  0.27786845100308710244  ,  0.28376817312891944312 ,  0.2896332925829483429  ,
    0.2954642128934210632  ,  0.30126133057638071477  ,  0.30702503529391833581 ,  0.31275571000333002303 ,
    0.31845373111718799919 ,  0.32411946865249774419  ,  0.32975328637076017912 ,  0.33535554191985283978 ,
    0.34092658697045408189 ,  0.34646676734519132879  ,  0.35197642315688426606 ,  0.35745588892132218461 ,
    0.36290549368823121767 ,  0.36832556115768966265  ,  0.37371640979290532414 ,  0.37907835293481184635 ,
    0.38441169890938908793 ,  0.38971675113862147555  ,  0.39499380824054242112 ,  0.40024316412564076018 ,
    0.4054651081078191055  ,  0.41065992498442938086  ,  0.41582789514359319583 ,  0.42096929464241839014 ,
    0.42608439531068142969 ,  0.43117346481813001446  ,  0.43623676677452749573 ,  0.44127456080423144158 ,
    0.44628710262804816011 ,  0.45127464413781126495  ,  0.45623743348005518783 ,  0.46117571512149879709 ,
    0.46608972992362396326 ,  0.47097971521725412458  ,  0.47584590486985689495 ,  0.48068852934557071421 ,
    0.48550781578160240315 ,  0.49030398804461583495  ,  0.49507726679621555377 ,  0.49982786955479241442 ,
    0.50455601075191225391 ,  0.50926190178870456293  ,  0.51394575110134610441 ,  0.51860776420653564855 ,
    0.52324814376333961263 ,  0.52786708962048578542  ,  0.53246479886911401991 ,  0.53704146589552692603 ,
    0.54159728243212157395 ,  0.54613243759740726091  ,  0.55064711795239418279 ,  0.55514150753879221156 ,
    0.55961578793539956678 ,  0.56407013828356866725  ,  0.56850473535087076016 ,  0.57291975356019975152 ,
    0.57731536503342795186 ,  0.5816917396332428325   ,  0.58604904500316479243 ,  0.59038744660210795701 ,
    0.59470710774621693417 ,  0.59900818964524660259  ,  0.60329085143712291028 ,  0.60755525022432266269 ,
    0.61180154110479634255 ,  0.61602987721380486619  ,  0.62024040975120442454 ,  0.62443328801055031363 ,
    0.62860865942093369085 ,  0.63276666957062843721  ,  0.63690746223619498778 ,  0.64103117942067910917 ,
    0.64513796137180179358 ,  0.64922794662379601505  ,  0.65330127201195864473 ,  0.65735807270721124951 ,
    0.6613984822452039225  ,  0.66542263254450517707  ,  0.66943065394116274547 ,  0.67342267521053145174 ,
    0.67739882359092007391 ,  0.68135922480723820627  ,  0.68530400309828110039 ,  0.68923328123855753802 ,
    0.69314718055829871446 
    };

    SYS_FORCEALIGN_16    STATIC const double ln_64f_trail_tbl[129] =
    {
    0                        ,  9.3248412463831087393e-13,  1.3659698372414652567e-12,  1.2941696325707852111e-12,
    4.5297099404706386849e-14,  1.2458922782343506697e-12,  1.3020418077047679617e-12,  6.5680100247433870209e-13,
    6.2998217753573726441e-13,  1.3460482728433476041e-12,  1.6549123182940661536e-12,  8.613942892310433308e-14 ,
    1.2906897772779757361e-12,  1.460623288984663759e-12 ,  7.5810191457748032917e-13,  1.6174145356373514915e-12,
    8.6294860146551854996e-13,  1.7434248489323067588e-12,  1.0114409310091332372e-12,  7.3629990993140381761e-13,
    1.7418011477587924674e-12,  1.0192957589083562198e-12,  8.8901108696859409974e-13,  1.2805312366026555537e-12,
    1.0503820035978606029e-12,  1.4929446567890636288e-12,  1.5421275367799580636e-12,  1.4286627436632670651e-12,
    6.1606275636449936428e-13,  1.2888301537117285989e-12,  1.363881230176389181e-12 ,  1.0006995232458848477e-12,
    1.0951795026414856693e-12,  1.33273947433565354e-12  ,  1.7258972029310370999e-12,  6.3388183590973312675e-13,
    4.4172998592273415852e-13,  1.1404766020461920562e-12,  2.3997470677272758621e-13,  1.0634271241372061922e-12,
    5.4078963529491375084e-13,  3.6920466683909580752e-13,  1.7251755579650307482e-12,  9.4313445941907048109e-14,
    4.1483483315118974133e-13,  1.78107528725490738e-12  ,  9.9353858473705258803e-13,  5.6687987637360492954e-13,
    1.3465895065678523679e-12,  1.7142398611724729562e-12,  1.7078005676296470483e-12,  1.2849721287011561799e-12,
    1.3911094498553211452e-13,  1.0172418463127996802e-12,  2.9393154576951019408e-13,  4.8161474808239290724e-13,
    1.1372569552747791022e-12,  1.0179634912788060319e-12,  6.7873484610458945099e-13,  1.5759615834554097091e-13,
    9.4296792596537670761e-13,  1.4037104811848166719e-12,  3.2657210269348979637e-13,  1.3719581026805371948e-12,
    3.4527936065842368407e-13,  8.389955397092307976e-13 ,  1.1779466291272910894e-13,  1.7112422590059850336e-12,
    2.1865842469992458064e-13,  2.4130697440227777406e-13,  3.9057646006313007092e-13,  6.4376282082889701996e-13,
    3.7136960173711486277e-13,  1.6472934127875760169e-12,  1.5323853297388723149e-12,  6.7135186299083215999e-13,
    9.7527541598196876294e-13,  1.5368817329886041989e-12,  1.070254995738650905e-13 ,  1.811883976188255474e-13 ,
    9.8421271133020127309e-14,  5.7798210661985649494e-13,  1.6359691379363994201e-12,  1.6568968419505836209e-12,
    4.8305803801440561074e-13,  1.1033396418724805699e-12,  8.8817841970012523234e-13,  1.5100143357926754106e-12,
    1.2082557176995578629e-12,  3.5660363550960028078e-13,  3.5782488083668795298e-13,  1.3566925360919412924e-12,
    6.2283511681471281918e-13,  7.2841732645656520617e-13,  2.6811886044697530451e-13,  1.7094103910153535253e-12,
    2.3092638912203256041e-14,  1.2343459587782490416e-12,  1.7980061883804410172e-12,  1.5857315460721110867e-12,
    1.395661364256284287e-12 ,  1.3796741527016820328e-12,  4.1344705437040829565e-13,  6.838973831690964289e-14 ,
    4.7584158835434209323e-13,  8.3677509365998048452e-13,  9.6134211702292304835e-13,  2.1915802506100590108e-13,
    1.1965983759409937193e-12,  1.709188346410428494e-12 ,  6.5314420538697959273e-13,  1.3431478151915143826e-12,
    1.4404033521486780955e-12,  4.0933922917929521645e-13,  8.7418960958984825993e-13,  2.5213164889237305033e-13,
    1.7829071552455388883e-12,  1.3138379273414102499e-12,  7.8703710215677347151e-13,  1.1487477635796494724e-12,
    1.6109336087311021402e-13,  5.8530957858238252811e-13,  1.4664935932273692742e-12,  1.6352474929703930684e-12,
    8.8606899595333743491e-13,  6.6491256944800625206e-13,  6.3826721685700249509e-13,  2.5146551507759795641e-13,
    1.6465717678215696651e-12
    };

}

//        Utils
IS float calcScale(int scaleFactor) 
{
    float scale = (scaleFactor > 0) ? 1.0F/(1 << scaleFactor): (1 << (-scaleFactor)) ; 
    return scale;
}

//        REFERENCE UNIT FUNCTIONS
 

ISV ln_8u_ref_unit(const Fw8u &src, Fw8u &dst, float scale)
{
    dst = (Fw8u)(LN_TABLES::ln_8u_tbl[src] * scale + 0.5F) ;    
}

//        REFERENCE CALLING FUNCTIONS

template <CH cd>
ISV Ln_8u_ref(const Fw8u *src,  Fw8u *dst, float scale)
{    
    for(int i=0 ; i< cd ; i++)
        ln_8u_ref_unit(src[i], dst[i], scale);        
}

template <class TS, class TD, CH cd>
ISV Ln_int_ref( const TS *src,  TD *dst, float scale )
{
    for(int i=0 ; i< cd ; i++){
        if(src[i] < 0)    
            dst[i] = FW_REF::Limits<TD>::MinValue() ;
        else
            dst[i] = FW_REF::Limits<TD>::Sat(FW_REF::float_int(logf((float)src[i])*scale ));
    }
}

template< class TS, class TD, CH cd >
ISV Ln_float_ref( const TS *src,  TD *dst)
{
    for(int i=0 ; i< cd ; i++)
        dst[i] = (TD)log(src[i]);
}

template< class TS, class TD, CH cd >
ISV Log10_float_ref(const TS *src,  TD *dst)
{    
    for(int i=0 ; i< cd ; i++)
        dst[i] = (TD)log10(src[i]);
}

ISV Log10_32s_ref( const Fw32s &src, Fw32s &dst, Fw32f scale )
{
    if(src <= 0)
        dst = FW_REF::Limits<A32S>::MinValue();
    else
        dst = FW_REF::Limits<A32S>::Sat(FW_REF::float_int((10*log10f((float)src))*scale )); //float_int
}

//        SSE2 SETUP FUNCTIONS

ISV ln_8u_setup(float scale, __m128 &m_scale)
{    
    m_scale = _mm_set1_ps(scale);
}

ISV ln_16s_scaleLT0_setup(__m128 &ln2, __m128i &exponent_127 )
{
    ln2             = _mm_set1_ps(0.69314718055994529F);    //log2    
    exponent_127    = _mm_set1_epi32(0x3F800000);           // 127 in exponent
}

ISV ln_16s_scale0_setup(__m128 &ln2, __m128i &exponent_127, __m128i &min_16s)
{
    ln2             = _mm_set1_ps(0.69314718055994529F);    //log2
    exponent_127    = _mm_set1_epi32(0x3F800000);           // 127 in exponent
    min_16s         = _mm_set1_epi16(-32768);               // min_16s (-32768)
}

// the constants are the jump points of the function ln_16s with scale 1
ISV ln_16s_scale1_setup(__m128i &twenty_1, __m128i &one_49, __m128i &one_097, __m128i &eight_104)
{
    twenty_1    = _mm_set1_epi16(21  ); 
    one_49      = _mm_set1_epi16(149 ); 
    one_097     = _mm_set1_epi16(1097); 
    eight_104   = _mm_set1_epi16(8104); 
}

// the constants are the jump points of the function ln_16s with scale 2
ISV ln_16s_scale2_setup(__m128i &eight, __m128i &four_04, __m128i &twenty_two_027)
{
    eight          = _mm_set1_epi16(8    ); 
    four_04        = _mm_set1_epi16(404  ); 
    twenty_two_027 = _mm_set1_epi16(22027); 
}    

// the constants are the jump points of the function ln_16s with scale>2
ISV ln_16s_scaleGT2_setup(int scaleFactor, __m128i &m_scale, 
                                    __m128i &scale3_const, __m128i &scale4_const, __m128i &four)
{
    m_scale      = _mm_set1_epi16((Fw16s)scaleFactor);
    scale3_const = _mm_set1_epi16(54); 
    scale4_const = _mm_set1_epi16(2980);    
    four         = _mm_set1_epi16(4);    
}

ISV ln_32f_setup(__m128 &hf_sqhf, __m128 &const1_const2, __m128 &p1_p2,
                                    __m128 &p3, __m128 &p4_p5, __m128 &p6_p7, __m128 &p8_p9)
{    //cephes
    hf_sqhf       = _mm_set_ps(0.5F, 0.5F, 0.7071067811865475F, 0.7071067811865475F);                        //1/2, sqrt(half)
    const1_const2 = _mm_set_ps(-2.12194440e-4F, -2.12194440e-4F, 0.693359375F, 0.693359375F);                //const1_const2
    p1_p2         = _mm_set_ps( 7.0376836292E-2F,  7.0376836292E-2F, -1.151461031E-1F, -1.151461031E-1F);    // p1_p2
    p3            = _mm_set1_ps(1.1676998740E-1F);                                                           //p3
    p4_p5         = _mm_set_ps(-1.2420140846E-1F, -1.2420140846E-1F, 1.4249322787E-1F, 1.4249322787E-1F);    //p4_p5
    p6_p7         = _mm_set_ps(-1.6668057665E-1F, -1.6668057665E-1F, 2.0000714765E-1F, 2.0000714765E-1F);    //p6_p7
    p8_p9         = _mm_set_ps(-2.4999993993E-1F, -2.4999993993E-1F, 3.3333331174E-1F, 3.3333331174E-1F);    //p8_p9
}

ISV ln_32s_setup(float scale, __m128 &m_scale, __m128 &ln_2, __m128i &exponent_127)
{    
    m_scale      = _mm_set1_ps( scale);
    ln_2         = _mm_set1_ps(0.69314718055994529F); //log2
    exponent_127 = _mm_set1_epi32(0x3F800000);
}


ISV ln_64f_setup(XMM128 &ln2_hi_lo, XMM128 &ln_const_1_2, XMM128 &ln_const_3_4, XMM128 &ln_const_5_6, XMM128 &ln_const_7_hexConsts )
{
    //const double    ln2_hi  =  6.93147180369123816490e-01,    /* 3fe62e42 fee00000 */
    //                ln2_lo  =  1.90821492927058770002e-10,    /* 3dea39ef 35793c76 */
    //                two54   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
    //                Lg1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
    //                Lg2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
    //                Lg3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
    //                Lg4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
    //                Lg5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
    //                Lg6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
    //                Lg7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

    ln2_hi_lo.d    = _mm_set_pd(6.93147180369123816490e-01, 1.90821492927058770002e-10);    //ln2_hi, ln2_lo
    ln_const_1_2.d = _mm_set_pd(6.666666666666735130e-01  , 3.999999999940941908e-01  );    //lg1, lg2
    ln_const_3_4.d = _mm_set_pd(2.857142874366239149e-01  , 2.222219843214978396e-01  );    //lg3, lg4
    ln_const_5_6.d = _mm_set_pd(1.818357216161805012e-01  , 1.531383769920937332e-01  );    //lg5, lg6

    ln_const_7_hexConsts.f64[0] = 1.479819860511658591e-01 ;                                //lg7,
    ln_const_7_hexConsts.s32[2] = 0x95f64;
    ln_const_7_hexConsts.s32[3] = 0x6147a ;
}

ISV ln_64f_tang_setup(__m128d &ln2_lead, __m128d &ln2_trail, __m128d &A1_A2)
{
    ln2_lead     = _mm_set1_pd(6.9314718055994450e-1 );          //log2_lead
    ln2_trail    = _mm_set1_pd(8.0034658537607260e-16);         //log2_trail
    A1_A2        = _mm_set_pd(0.08333333333333333, 0.0125);     //A1_A2
}

//fixed accuracy stuff

ISV ln_A21_setup(__m128 &ln2_lead, __m128 &ln2_trail, __m128i &exponent_127, __m128 &A1_A2)
{
    ln2_lead     = _mm_set1_ps(0.693145751953125F);
    ln2_trail    = _mm_set1_ps(1.4286068202862268e-06F);
    exponent_127 = _mm_set1_epi32(0x3F800000);
    A1_A2        = _mm_set_ps( 0.333329854F,  0.333329854F, -0.500003886F, -0.500003886F);  //paper values : 0.333329854, -0.500003886
}

ISV log10_A24_setup(__m128 &hf_sqrthf, __m128 &ln10_2_econsts, 
                                       __m128 &p1_p2, __m128 &p3, __m128 &p4_p5,
                                       __m128 &p6_p7, __m128 &p8_p9)
{
    hf_sqrthf      = _mm_set_ps(0.5F, 0.5F, 0.7071067811865475F, 0.7071067811865475F);      //1/2, sqrt(half)
    ln10_2_econsts = _mm_set_ps(3.0078125e-1f, 2.48745663981195213739e-4f,
                            4.3359375e-1f, 7.00731903251767651129e-4f);                     //l10_2head, l10_2trail, l10_ehead, l10_etrail
    p1_p2    = _mm_set_ps( 7.0376836292e-2F,  7.0376836292e-2F, -1.151461031e-1F, -1.151461031e-1F);
    p3       = _mm_set1_ps(1.1676998740e-1F);
    p4_p5    = _mm_set_ps(-1.2420140846e-1F, -1.2420140846e-1F, 1.4249322787e-1F, 1.4249322787e-1F);
    p6_p7    = _mm_set_ps(-1.6668057665e-1F, -1.6668057665e-1F, 2.0000714765e-1F, 2.0000714765e-1F);
    p8_p9    = _mm_set_ps(-2.4999993993e-1F, -2.4999993993e-1F, 3.3333331174e-1F, 3.3333331174e-1F);
}

ISV tenLog10_32s_setup(float scale, __m128 &logE_base10xscalex10, __m128 &ln2_lead, __m128 &ln2_trail, __m128 &A1_A2 )
{
    logE_base10xscalex10 = _mm_set1_ps(scale* 4.3429448190325176F);     // 10*logE_base10xscale
    ln2_lead             = _mm_set1_ps(0.693145751953125F);             //log2_lead
    ln2_trail            = _mm_set1_ps(0.000001430511475F);             //log2_trail
    A1_A2                = _mm_set_ps(1/3.0F, 1/3.0F, -0.5F, -0.5F);
}

//        SSE2 UNIT FUNCTIONS     


ISV ln_8u_unit(const XMM128 &src, XMM128 &dst, const __m128 &scale)
{
    XMM128 a, b, c, d;

    Fw32u i0 = src.u32[0] ;
    Fw32u i1 = src.u32[1] ;
    Fw32u i2 = src.u32[2] ;
    Fw32u i3 = src.u32[3] ;

        //Look up the table values
        a.f = _mm_set_ps(LN_TABLES::ln_8u_tbl[(Fw8u)(i0>>24)], LN_TABLES::ln_8u_tbl[(Fw8u)(i0>>16)],
                         LN_TABLES::ln_8u_tbl[(Fw8u)(i0>>8)] , LN_TABLES::ln_8u_tbl[(Fw8u)(i0)]);
        a.f = _mm_mul_ps(a.f, scale);    a.i = _mm_cvtps_epi32(a.f); 

        b.f = _mm_set_ps(LN_TABLES::ln_8u_tbl[(Fw8u)(i1>>24)], LN_TABLES::ln_8u_tbl[(Fw8u)(i1>>16)],
                         LN_TABLES::ln_8u_tbl[(Fw8u)(i1>>8)] , LN_TABLES::ln_8u_tbl[(Fw8u)(i1)]);
        b.f = _mm_mul_ps(b.f, scale);    b.i = _mm_cvtps_epi32(b.f);

        c.f = _mm_set_ps(LN_TABLES::ln_8u_tbl[(Fw8u)(i2>>24)], LN_TABLES::ln_8u_tbl[(Fw8u)(i2>>16)],
                         LN_TABLES::ln_8u_tbl[(Fw8u)(i2>>8)] , LN_TABLES::ln_8u_tbl[(Fw8u)(i2)]);
        c.f = _mm_mul_ps(c.f, scale);    c.i = _mm_cvtps_epi32(c.f);

        d.f = _mm_set_ps(LN_TABLES::ln_8u_tbl[(Fw8u)(i3>>24)], LN_TABLES::ln_8u_tbl[(Fw8u)(i3>>16)],
                         LN_TABLES::ln_8u_tbl[(Fw8u)(i3>>8)] , LN_TABLES::ln_8u_tbl[(Fw8u)(i3)]);
        d.f = _mm_mul_ps(d.f, scale);    d.i = _mm_cvtps_epi32(d.f);

        //Saturate pack 8 32-bit integer to 8 16-bit integer
        a.i =_mm_packs_epi32(a.i, b.i );
        c.i =_mm_packs_epi32(c.i, d.i );

        //Saturate pack 16 16-bit integer to 16 8-bit unsigned char 
        // and store the reslt
        dst.i =_mm_packus_epi16(a.i, c.i);
}

//16s

// To use in integer versions for ln_16s, ln_32s, ln_32sI, ln_32s16s etc...
// expects src.f has the int converted into float

ISV ln_lowAccuracy_unit(const XMM128 &src, XMM128 &dst,
                                   const __m128 &ln2, const __m128i &one_twenty7)
{
    XMM128 m, j, f_lead, f_trail;
        
    m.i = _mm_sub_epi32(src.i, one_twenty7);
    m.i = _mm_srai_epi32(m.i, 23);
    m.f = _mm_cvtepi32_ps(m.i);                         //  1 <= 2^(-m.i)*X < 2 
    
    // log = m* log2 + c_total(j) + f_trail/f_lead
    dst.f = _mm_mul_ps(ln2, m.f);

    j.i = _mm_srli_epi32(_mm_slli_epi32(src.i, 9), 9) ; // mantissa part    
    f_trail.i = _mm_or_si128(j.i, one_twenty7);
    
    //  7 bits of mantissa of src, excluding significand 
    j.i = _mm_srli_epi32(j.i, 16);

    // f_lead contains first 7 significant bits of mantissa of src => f_lead = 1 + j.i/128
    f_lead.i = _mm_slli_epi32( j.i, 16); 
    f_lead.i = _mm_or_si128(f_lead.i, one_twenty7);
    
    // f_trail contains the difference of mantissa and f_lead
    f_trail.f = _mm_sub_ps(f_trail.f, f_lead.f);    
    
    m.f = _mm_set_ps(   LN_TABLES::ln_16s_tbl[_mm_extract_epi16(j.i, 6)],  //j.u32[3]],
                        LN_TABLES::ln_16s_tbl[_mm_extract_epi16(j.i, 4)],  //j.u32[2]],
                        LN_TABLES::ln_16s_tbl[_mm_extract_epi16(j.i, 2)],  //j.u32[1]],
                        LN_TABLES::ln_16s_tbl[_mm_extract_epi16(j.i, 0)]); //j.u32[0]])); 
    dst.f = _mm_add_ps(dst.f, m.f);

    j.f = _mm_mul_ps(f_trail.f , _mm_rcp_ps(f_lead.f));
    dst.f = _mm_add_ps(dst.f, j.f);
}

//scale <0
ISV log_16s_slt0(const XMM128 &src, XMM128 &dst, 
                                   const __m128 &ln2, const __m128i &one_twenty7, int scale)
{    
    XMM128 tsrc; 
    
    tsrc.f = _mm_cvtepi32_ps  (src.i);
    while(scale != 0){
        tsrc.f  = _mm_mul_ps(tsrc.f, tsrc.f);
        scale++ ;
    }
    ln_lowAccuracy_unit(tsrc, dst, ln2, one_twenty7);
    dst.i = _mm_cvtps_epi32(dst.f);
    
}

ISV ln_16s_scaleLT0_unit(const XMM128 &src, XMM128 &dst, 
                                        const __m128 &log2, const __m128i &one_twenty7, const int &scale)
{
    XMM128 src_l, src_h, dst_l, dst_h ;
    const __m128i zero =  _mm_setzero_si128();

    src_l.i = _mm_max_epi16(src.i, zero);
    CBL_SSE2::Unpack16STo32S(src_l.i, src_h.i);

    log_16s_slt0(src_l, dst_l, log2, one_twenty7, scale);    
    log_16s_slt0(src_h, dst_h, log2, one_twenty7, scale);

    dst_l.i = _mm_packs_epi32(dst_l.i,dst_h.i);
    
    // if src >0 , dst<- dst else dst <- MIN_16S
    __m128i min_16s = _mm_slli_epi16(_mm_cmpeq_epi16(zero, zero), 15);    //0x8000
    src_l.i = _mm_cmpgt_epi16(src.i, zero);
    src_l.i = _mm_xor_si128(src_l.i, min_16s);
    dst.i   = _mm_and_si128(dst_l.i, src_l.i);
}

ISV ln_16s_scale0(XMM128 &src, XMM128 &dst, const __m128 &ln2, const __m128i &one_twenty7)
{    
    src.f = _mm_cvtepi32_ps(src.i);
    ln_lowAccuracy_unit(src, dst, ln2, one_twenty7);
    dst.i = _mm_cvtps_epi32(dst.f);
}


ISV ln_16s_scale0_unit(const XMM128 &src, XMM128 &dst, const __m128 &log2, const __m128i &one_twenty7, const __m128i &min_16s)
{
    XMM128 src_l, src_h, dst_l, dst_h ;
    const __m128i zero =  _mm_setzero_si128();

    src_l.i = _mm_max_epi16(src.i, zero);
    
    src_h.i = _mm_unpackhi_epi16( src_l.i, zero ); 
    src_l.i = _mm_unpacklo_epi16( src_l.i, zero ); 

    ln_16s_scale0(src_l, dst_l, log2, one_twenty7);    
    ln_16s_scale0(src_h, dst_h, log2, one_twenty7);

    dst_l.i = _mm_packs_epi32(dst_l.i,dst_h.i);

    // if src >0 , dst<- dst else dst <- MIN_16S
    src_l.i = _mm_cmpgt_epi16(src.i, zero);
    src_l.i = _mm_xor_si128(src_l.i, min_16s);
    dst.i   = _mm_and_si128(dst_l.i, src_l.i);
}

ISV ln_16s_scale1_unit( const XMM128 &src, XMM128 &dst, 
                                   const __m128i &twenty_1, const __m128i &one_49, const __m128i &one_097, const __m128i &eight_104)
{
    __m128i cmp1, cmp2, cmp3, tdst, tmp ;

    tdst = _mm_cmplt_epi16(src.i, _mm_srli_epi16(twenty_1, 4));     // src < 1    => dst = -32768
    tdst = _mm_slli_epi16(_mm_srli_epi16(tdst, 15), 15);

    cmp1 = _mm_cmplt_epi16(src.i, _mm_srli_epi16(eight_104, 11));   //cmp 3
    cmp2 = _mm_cmplt_epi16(src.i, twenty_1);
    cmp3 = _mm_cmplt_epi16(src.i, one_49);

    tmp  = _mm_andnot_si128(cmp1, cmp2);                             // 3 < = src < 21 => dst = 1, otherwise 0
    tmp  = _mm_srli_epi16(tmp, 15);        
    tdst = _mm_or_si128(tdst, tmp);
    
    tmp  = _mm_andnot_si128(cmp2, cmp3);                             //  21 <= src < 149 => dst = 2
    tmp  = _mm_slli_epi16(_mm_srli_epi16(tmp, 15), 1);
    tdst = _mm_or_si128(tdst, tmp);

    cmp1 = _mm_cmplt_epi16(src.i, one_097);
    cmp2 = _mm_cmplt_epi16(src.i, eight_104);

    tmp  = _mm_andnot_si128(cmp3, cmp1);                             //  149 <= src < 1097 => dst = 3
    tmp  = _mm_srli_epi16(tmp, 14);
    tdst = _mm_or_si128(tdst, tmp);

    tmp  = _mm_andnot_si128(cmp1, cmp2);                             //  1097 <= src < 8104 => dst = 4
    tmp  = _mm_slli_epi16(_mm_srli_epi16(tmp, 15), 2);
    tdst = _mm_or_si128(tdst, tmp);

    tmp   = _mm_andnot_si128(cmp2, _mm_cmpeq_epi16(cmp2, cmp2));
    tmp   = _mm_and_si128(tmp, _mm_srli_epi16(twenty_1, 2));         // src > 8104 => dst = 5
    dst.i = _mm_or_si128(tdst, tmp);
}

ISV ln_16s_scale2_unit(const XMM128 &src, XMM128 &dst, 
                                   const __m128i &eight, const __m128i &four_04, const __m128i &twenty_two_027)
{
    __m128i cmp1, cmp2, cmp3, tdst ;

    cmp1 = _mm_cmplt_epi16(src.i, four_04);
    cmp2 = _mm_cmplt_epi16(src.i, eight);
    cmp3 = _mm_cmplt_epi16(src.i, twenty_two_027);

    tdst = _mm_cmplt_epi16(src.i, _mm_srli_epi16(eight, 3));         // src < 1    => dst = -32768
    tdst = _mm_slli_epi16(_mm_srli_epi16(tdst, 15), 15);
    
    __m128i tmp;
    tmp  = _mm_andnot_si128(cmp2, cmp1);                             // 8 < = src < 404 => dst = 1, otherwise 0
    tmp  = _mm_srli_epi16(tmp, 15);        
    tdst = _mm_or_si128(tdst, tmp);
    
    tmp  = _mm_andnot_si128(cmp1, cmp3);                             //  404 <= src < 22027 => dst = 2
    tmp  = _mm_slli_epi16(_mm_srli_epi16(tmp, 15), 1);
    tdst = _mm_or_si128(tdst, tmp);

    tmp   = _mm_andnot_si128(cmp3, _mm_cmpeq_epi16(cmp3, cmp3));
    tmp   = _mm_srli_epi16(tmp, 14);                                 // src > 22027 => dst = 3
    dst.i = _mm_or_si128(tdst, tmp);
}


ISV ln_16s_scaleGT2_unit(const XMM128 &src, XMM128 &dst, const __m128i &scale, 
                                   const __m128i &scale3_const, const __m128i &scale4_const, const __m128i &four)
{
    __m128i tmp1, tmp2, one;

    one = _mm_srli_epi16(four, 2);

    tmp1 = _mm_cmpeq_epi16(scale, _mm_sub_epi16(four, one));
    tmp1 = _mm_and_si128(tmp1, _mm_cmpgt_epi16(src.i, scale3_const));
    
    tmp2 = _mm_cmpeq_epi16(scale, four);
    tmp2 = _mm_and_si128(tmp2, _mm_cmpgt_epi16(src.i, scale4_const));
    tmp1 = _mm_or_si128(tmp1, tmp2);
    tmp1 = _mm_and_si128(tmp1, one);

    tmp2 = _mm_cmplt_epi16(src.i, one);
    tmp2 = _mm_and_si128(tmp2, _mm_slli_epi16(one, 15));
    tmp1 = _mm_or_si128(tmp1, tmp2);

    tmp2  = _mm_cmplt_epi16(scale, _mm_add_epi16(four, one));
    dst.i = _mm_and_si128(tmp2, tmp1);
}

ISV ln_32f_unit(const XMM128 &src, XMM128 &dst, 
                                     const __m128 &half_sqhalf, const __m128 &const1_const2,
                                     const __m128 &p1_p2      , const __m128 &p3, 
                                     const __m128 &p4_p5      , const __m128 &p6_p7, const __m128 &p8_p9)
{
    XMM128 x, one, half;
    __m128 y, z  , sqrt_half ; 
    XMM128 e, cmp ;

    half.f    = CBL_OPT::Shuffle_a3a2b3b2_32f(half_sqhalf, half_sqhalf );
    one.f     = _mm_add_ps(half.f, half.f);
    sqrt_half = CBL_OPT::Shuffle_b1b0a1a0_32f (half_sqhalf, half_sqhalf );

    e.i = _mm_sub_epi32(src.i, one.i);
    e.i = _mm_srai_epi32(e.i, 23);
    e.f = _mm_cvtepi32_ps(e.i);            //  0 <= 2^(-e.f)*src < 1 
    e.f = _mm_add_ps(e.f, one.f);

    x.i = _mm_srli_epi32(_mm_slli_epi32(src.i, 9), 9);
    x.i = _mm_or_si128(x.i, half.i);

    y     = _mm_sub_ps(x.f, one.f);
    cmp.f = _mm_cmplt_ps(x.f, sqrt_half);
    e.f   = _mm_sub_ps(e.f, _mm_and_ps(cmp.f, one.f));
    x.i   = _mm_and_si128(x.i, cmp.i);
    x.f   = _mm_add_ps(x.f, y);

    z = _mm_mul_ps(x.f, x.f);

    y = _mm_mul_ps(x.f, CBL_OPT::Shuffle_a3a2b3b2_32f (p1_p2, p1_p2)/*p1*/);
    y = _mm_add_ps(y,   CBL_OPT::Shuffle_b1b0a1a0_32f (p1_p2, p1_p2)/*p2*/);
    y = _mm_mul_ps(y, x.f);
    y = _mm_add_ps(y, p3);
    y = _mm_mul_ps(y, x.f);
    y = _mm_add_ps(y, CBL_OPT::Shuffle_a3a2b3b2_32f (p4_p5, p4_p5)/*p4*/);
    y = _mm_mul_ps(y, x.f);
    y = _mm_add_ps(y, CBL_OPT::Shuffle_b1b0a1a0_32f (p4_p5, p4_p5)/*p5*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_a3a2b3b2_32f (p6_p7, p6_p7)/*p6*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_b1b0a1a0_32f (p6_p7, p6_p7)/*p7*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_a3a2b3b2_32f (p8_p9, p8_p9)/*p8*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_b1b0a1a0_32f (p8_p9, p8_p9)/*p9*/);

    y = _mm_mul_ps(y, x.f);
    y = _mm_mul_ps(y, z);

    __m128 tmp;
    tmp = _mm_mul_ps(CBL_OPT::Shuffle_a3a2b3b2_32f(const1_const2, const1_const2)/*const1*/, e.f);
    y   = _mm_add_ps(y, tmp);
    tmp = _mm_mul_ps(z, half.f);
    y   = _mm_sub_ps(y, tmp);

    z     = _mm_add_ps(x.f, y);
    tmp   = _mm_mul_ps(CBL_OPT::Shuffle_b1b0a1a0_32f (const1_const2, const1_const2)/*const2*/, e.f);
    dst.f = _mm_add_ps(z, tmp);
}

ISV ln_32f_special_case(const XMM128 &src, XMM128 &dst)
{
    /* (1)if src < 0, dst <-NAN_32F (0xFFC00000)
       (2)if src < MIN_ABS_32F(1.1754944e-38 i.e., 0x00800000), dst <- INF_NEG_32F (0xFF800000)
       else dst <- q */

    XMM128 m, n;
    m.i = _mm_cmpeq_epi32(src.i, src.i);
    m.i = _mm_slli_epi32(_mm_srli_epi32(m.i, 31), 23);                  //0x00800000
    m.i = _mm_cmplt_epi32(src.i, m.i);
    n.i = _mm_andnot_si128( m.i, dst.i);
    m.i = _mm_slli_epi32(_mm_srli_epi32(m.i, 23), 23);                  //0xFF800000;
    m.i = _mm_or_si128(m.i, n.i);

    n.i   = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 22);              //get sign bit    // (1) 
    dst.i = _mm_xor_si128(m.i, n.i);    
}

ISV  ln_32s_special_case(const __m128i &src, __m128i &dst)
{
    // if src >0 , dst<- dst else dst <- MIN_32S
    __m128i cmp = _mm_cmpgt_epi32(src, _mm_setzero_si128());
    __m128i min_32s = _mm_slli_epi32(_mm_cmpeq_epi32(src, src), 31);    //0x80000000
    cmp = _mm_xor_si128(cmp, min_32s);
    dst = _mm_and_si128(dst, cmp);
}

ISV ln_64f_unit(const XMM128 &src, XMM128 &dst, const __m128d &ln2_hi_lo, const __m128d &lg1_lg2,
                                    const __m128d &lg3_lg4, const __m128d &lg5_lg6, const XMM128 &lg7_c1_c2)
{
    XMM128 x ;
    XMM128 one ;
    one.d = _mm_set1_pd(1.0);                                                                   //0x3ff00000, 0 , 0x3ff00000, 0 
    
    __m128i k  = _mm_srai_epi32(_mm_sub_epi32(src.i, one.i), 20);                               //k += (hx>>20)-1023;
    __m128i hx = _mm_srli_epi64(_mm_slli_epi64(src.i, 12), 12);                                 //hx &= 0x000fffff;
    __m128i i  = _mm_add_epi32(hx, _mm_shuffle_epi32(lg7_c1_c2.i, _MM_SHUFFLE(2, 2, 2, 2)));    //i = (hx+0x95f64)&0x100000;
            i  = _mm_slli_epi64(_mm_srli_epi64(i, 52), 52);
           x.i = _mm_or_si128(hx, _mm_xor_si128(i, one.i));                                     //x.i[HI] = hx|(i^0x3ff00000);
           k   = _mm_add_epi32(k, _mm_srli_epi32(i, 20));                                       //    k += (i>>20);
    __m128d f  = _mm_sub_pd(x.d, one.d);                                                        //    f = x.d-1.0;
    __m128d s  = _mm_add_pd(x.d, one.d);
            s  = _mm_div_pd(f, s);                                                              //    s = f/(2.0+f) => f/(1.0 +x);
            k  = _mm_shuffle_epi32(k, _MM_SHUFFLE( 2, 0, 3, 1));
    __m128d dk = _mm_cvtepi32_pd(k);                                                            //    dk = (double)k;
    __m128d z  = _mm_mul_pd(s, s);                                                              //    z = s*s;
            i  = _mm_sub_epi32(hx,  _mm_shuffle_epi32(lg7_c1_c2.i, _MM_SHUFFLE(3, 3, 3, 3)));   // i = hx-0x6147a;
    __m128d w  = _mm_mul_pd(z, z);                                                              //    w = z*z;
    __m128i j  = _mm_sub_epi32(_mm_set_epi32(0x6b851, 0, 0x6b851, 0), hx);                      //j = 0x6b851-hx;

    //        t1= w*(Lg2+w*(Lg4+w*Lg6));        
    __m128d t1 = _mm_unpacklo_pd(lg5_lg6, lg5_lg6);                                             //lg6
    t1 = _mm_mul_pd(t1, w);
    t1 = _mm_add_pd(t1, _mm_unpacklo_pd(lg3_lg4, lg3_lg4));                                     //lg4
    t1 = _mm_mul_pd(t1, w);
    t1 = _mm_add_pd(t1, _mm_unpacklo_pd(lg1_lg2, lg1_lg2));                                     //lg2
    t1 = _mm_mul_pd(t1, w);

    //    t2= z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7)));    
    __m128d t2 = _mm_unpacklo_pd(lg7_c1_c2.d, lg7_c1_c2.d);                                     //lg7
    t2 = _mm_mul_pd(t2, w);
    t2 = _mm_add_pd(t2, _mm_unpackhi_pd(lg5_lg6, lg5_lg6));                                     //lg5
    t2 = _mm_mul_pd(t2, w);
    t2 = _mm_add_pd(t2, _mm_unpackhi_pd(lg3_lg4, lg3_lg4));                                     //lg3    
    t2 = _mm_mul_pd(t2, w);
    t2 = _mm_add_pd(t2, _mm_unpackhi_pd(lg1_lg2, lg1_lg2));                                     //lg1    
    t2 = _mm_mul_pd(t2, z);

            i = _mm_or_si128(i, j);                                                             //    i |= j;
    __m128d R = _mm_add_pd(t1, t2);                                                             //    R = t2+t1;

    XMM128 hfsq ;
    hfsq.d = _mm_mul_pd(f, f);
    hfsq.i = _mm_sub_epi64(hfsq.i, _mm_srli_epi32(_mm_slli_epi32(one.i, 11), 11));              //hfsq=0.5*f*f;

    XMM128 cmp ;
    cmp.i = _mm_cmpgt_epi32(i, _mm_setzero_si128());
    cmp.i = _mm_shuffle_epi32(cmp.i, _MM_SHUFFLE(2, 2, 0, 0));

   //        hfsq-s*(hfsq+R) 
    __m128d igt0 = _mm_add_pd(hfsq.d, R);    
    igt0 = _mm_mul_pd(s, igt0);
    igt0 = _mm_sub_pd(hfsq.d, igt0);
    igt0 = _mm_and_pd(igt0, cmp.d);

    //    s*(f-R)    
    __m128d ile0 = _mm_sub_pd(f, R);
    ile0 = _mm_mul_pd(ile0, s);
    ile0 = _mm_andnot_pd(cmp.d, ile0);

    x.d = _mm_or_pd(ile0, igt0);

    __m128d lo = _mm_mul_pd(dk, _mm_unpacklo_pd(ln2_hi_lo, ln2_hi_lo));
    __m128d hi = _mm_mul_pd(dk, _mm_unpackhi_pd(ln2_hi_lo, ln2_hi_lo));
           x.d = _mm_sub_pd(x.d, lo);
           x.d = _mm_sub_pd(x.d, f);
         dst.d = _mm_sub_pd(hi, x.d);
}

ISV ln_64f_special_case(const XMM128 &src, XMM128 &dst)
{    
    // (1)if src < 0, dst <-NAN_64F      (0x7ff8000000000000)
    // (2)if src = 0, dst <- INF_NEG_64F (0xfff0000000000000)
    // else dst <- q */

    XMM128 cmp ;

          cmp.d = _mm_cmple_pd(src.d, _mm_setzero_pd());
          dst.i = _mm_andnot_si128(cmp.i, dst.i); 
          cmp.i = _mm_slli_epi64(_mm_srli_epi64(cmp.i, 52), 52);                       //0xfff0000000000000
    __m128i tmp = _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 51);                       //put the sign bit to get nan_64f
            tmp = _mm_or_si128(tmp, _mm_slli_epi64(tmp, 12));
          cmp.i = _mm_xor_si128(cmp.i, tmp);
          dst.i = _mm_or_si128(dst.i, cmp.i);
}

ISV ln_64f_tang(const XMM128 &src, XMM128 &dst, const __m128d &log2_lead, const __m128d &log2_trail,
                                    const __m128d &A1_A2)
{
    // Implementation is based on the paper :
    // Table driven implementation of the Logarithm function in 
    // IEEE floating point arithmetic -- PING TAK PETER TANG 

    XMM128 m, f_lead, f_trail ; 
    __m128d q, u, l_lead, l_trail ;
    __m128i j;
    const __m128i thousand_23 = _mm_srli_epi64(_mm_cmpeq_epi32(src.i, src.i), 54);      // 0, 1023, 0, 1023
    XMM128 one ;
    one.i = _mm_slli_epi64(thousand_23, 52);  
 
    m.i = _mm_srli_epi64(src.i, 52);
    m.i = _mm_sub_epi64(m.i, thousand_23);                                              //  1 <= 2^(-m.i)*X < 2 
    m.i = _mm_shuffle_epi32(m.i, _MM_SHUFFLE(3,1,2,0));
    m.d = _mm_cvtepi32_pd(m.i);

    j = _mm_srli_epi64(_mm_slli_epi64(src.i, 12), 12) ;                                 // mantissa part    
    f_trail.i = _mm_or_si128(j, one.i);
    
    //  7 bits of mantissa of src, excluding significand 
    j = _mm_srli_epi64(j, 45);

    // f_lead contains first 7 significant bits of mantissa of src => f_lead = 1 + j/128
    f_lead.i = _mm_slli_epi64( j, 45); 
    f_lead.i = _mm_or_si128(f_lead.i, one.i);
    
    // f_trail contains the difference of mantissa and f_lead
    f_trail.d = _mm_sub_pd(f_trail.d, f_lead.d); 

    int index1 = _mm_extract_epi16(j,4); 
    int index0 = _mm_extract_epi16(j,0);

    l_lead = _mm_mul_pd(log2_lead, m.d);
    l_lead = _mm_add_pd(l_lead, _mm_set_pd(LN_TABLES::ln_64f_lead_tbl[index1],
                                            LN_TABLES::ln_64f_lead_tbl[index0]));

    l_trail = _mm_mul_pd(log2_trail, m.d);
    l_trail = _mm_add_pd(l_trail, _mm_set_pd(LN_TABLES::ln_64f_trail_tbl[index1],
                                             LN_TABLES::ln_64f_trail_tbl[index0]));

    // u = (f+f)/(2F+f) ; v = u*u ; q = u*v*(A1 + v A2 )

    u = _mm_add_pd(f_trail.d, f_trail.d);
    q = _mm_add_pd(f_lead.d, f_lead.d);
    q = _mm_add_pd(q, f_trail.d);
    u = _mm_div_pd(u, q);

    f_lead.d = _mm_mul_pd(u, u);    //v
    q  = _mm_mul_pd(f_lead.d, _mm_unpacklo_pd(A1_A2, A1_A2)/*A2*/);
    q = _mm_add_pd(q, _mm_unpackhi_pd(A1_A2, A1_A2)/*A1*/);
    q = _mm_mul_pd(q, f_lead.d); 
    q = _mm_mul_pd(q, u);

    // result = l_lead +(u + (q+l_trail)) 
    
    q = _mm_add_pd(l_trail, q);
    q = _mm_add_pd(q, u);
    dst.d = _mm_add_pd(q, l_lead);    
}

//fixed accuracy stuff

ISV ln_A21_unit(const XMM128 &src, XMM128 &dst, const __m128 &log2_lead, const __m128 &log2_trail,
                                     const __m128i &one_twenty7, const __m128 &A1_A2)
{
    XMM128 m, f_lead, f_trail; 
    __m128 q, u,  l_lead, l_trail ;
    XMM128 j;
    
    m.i = _mm_sub_epi32(src.i, one_twenty7);
    m.i = _mm_srai_epi32(m.i, 23);
    m.f = _mm_cvtepi32_ps(m.i);                                     //  1 <= 2^(-m.i)*X < 2 

    j.i = _mm_srli_epi32(_mm_slli_epi32(src.i, 9), 9) ;             // mantissa part    
    f_trail.i = _mm_or_si128(j.i, one_twenty7);
    
    // 7 bits of mantissa of src, excluding significand 
    j.i = _mm_srli_epi32(j.i, 16);

    // f_lead contains first 7 significant bits of mantissa of src => f_lead = 1 + j.i/128
    f_lead.i = _mm_slli_epi32( j.i, 16); 
    f_lead.i = _mm_or_si128(f_lead.i, one_twenty7);
    
    // f_trail contains the difference of mantissa and f_lead
    f_trail.f = _mm_sub_ps(f_trail.f, f_lead.f);    

    // u = f/F ; q = u * u * (A1 + u*A2) 
    u = _mm_div_ps(f_trail.f , f_lead.f );
    // u = _mm_mul_ps(f_trail.f , _mm_rcp_ps(f_lead.f ));
    q = _mm_mul_ps(u, CBL_OPT::Shuffle_a3a2b3b2_32f(A1_A2, A1_A2)/*A1: 1/3*/ );
    q = _mm_add_ps(q, CBL_OPT::Shuffle_b1b0a1a0_32f (A1_A2, A1_A2)/*A2: -0.5*/ );
    q = _mm_mul_ps(q, u);
    q = _mm_mul_ps(q, u);

    int i0 = _mm_extract_epi16(j.i, 0);    //j.u16[0]; 
    int i1 = _mm_extract_epi16(j.i, 2);    //j.u16[2]; 
    int i2 = _mm_extract_epi16(j.i, 4);    //j.u16[4]; 
    int i3 = _mm_extract_epi16(j.i, 6);    //j.u16[6]; 

    // l_lead = m* log2_lead + c_lead(j)  
    l_lead = _mm_mul_ps(log2_lead , m.f);
    f_lead.f = _mm_set_ps( CBL_LN_TABLES::ln_32f_lead_tbl[i3], CBL_LN_TABLES::ln_32f_lead_tbl[i2], 
                           CBL_LN_TABLES::ln_32f_lead_tbl[i1], CBL_LN_TABLES::ln_32f_lead_tbl[i0]);  
    l_lead = _mm_add_ps(l_lead, f_lead.f);
            
    // l_trail = m* log2_trail + c_trail(j) 
    l_trail = _mm_mul_ps(log2_trail, m.f);
    f_trail.f = _mm_set_ps(CBL_LN_TABLES::ln_32f_trail_tbl[i3], CBL_LN_TABLES::ln_32f_trail_tbl[i2],
                           CBL_LN_TABLES::ln_32f_trail_tbl[i1], CBL_LN_TABLES::ln_32f_trail_tbl[i0]); 
    l_trail = _mm_add_ps(l_trail, f_trail.f);

    // result = l_lead +(u + (q+l_trail)) 
    q = _mm_add_ps(l_trail, q);
    q = _mm_add_ps(q, u);
    dst.f = _mm_add_ps(q, l_lead);
}


ISV log10_32f_unit(const XMM128 &src, XMM128 &dst, const __m128 &half_sqhalf, const __m128 &base_consts,
                                     const __m128 &p1_p2, const __m128 &p3, const __m128 &p4_p5, const __m128 &p6_p7, const __m128 &p8_p9)
{
    XMM128 x, one, half;
    __m128 y, z  , sqrt_half ; 
    XMM128 e, cmp ;
     
    half.f = CBL_OPT::Shuffle_a3a2b3b2_32f(half_sqhalf, half_sqhalf );
    one.f = _mm_add_ps(half.f, half.f);
    sqrt_half = CBL_OPT::Shuffle_b1b0a1a0_32f (half_sqhalf, half_sqhalf );

    e.i = _mm_sub_epi32(src.i, one.i);
    e.i = _mm_srai_epi32(e.i, 23);
    e.f = _mm_cvtepi32_ps(e.i);                                     //  0 <= 2^(-e.f)*src < 1 
    e.f = _mm_add_ps(e.f, one.f);

    x.i = _mm_srli_epi32(_mm_slli_epi32(src.i, 9), 9);
    x.i = _mm_or_si128(x.i, half.i);

    y     = _mm_sub_ps(x.f, one.f);
    cmp.f = _mm_cmplt_ps(x.f, sqrt_half);
    e.f   = _mm_sub_ps(e.f, _mm_and_ps(cmp.f, one.f));
    x.i   = _mm_and_si128(x.i, cmp.i);
    x.f   = _mm_add_ps(x.f, y);

    z = _mm_mul_ps(x.f, x.f);

    y = _mm_mul_ps(x.f, CBL_OPT::Shuffle_a3a2b3b2_32f (p1_p2, p1_p2)/*p1*/);
    y = _mm_add_ps(y,   CBL_OPT::Shuffle_b1b0a1a0_32f (p1_p2, p1_p2) /*p2*/);
    y = _mm_mul_ps(y, x.f);
    y = _mm_add_ps(y, p3);
    y = _mm_mul_ps(y, x.f);
    y = _mm_add_ps(y, CBL_OPT::Shuffle_a3a2b3b2_32f (p4_p5, p4_p5)/*p4*/);
    y = _mm_mul_ps(y, x.f);
    y = _mm_add_ps(y, CBL_OPT::Shuffle_b1b0a1a0_32f (p4_p5, p4_p5)/*p5*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_a3a2b3b2_32f (p6_p7, p6_p7)/*p6*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_b1b0a1a0_32f (p6_p7, p6_p7)/*p7*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_a3a2b3b2_32f (p8_p9, p8_p9)/*p8*/);
    y = _mm_mul_ps(y, x.f);                  
    y = _mm_add_ps(y, CBL_OPT::Shuffle_b1b0a1a0_32f (p8_p9, p8_p9)/*p9*/);
    y = _mm_mul_ps(y, x.f);
    y = _mm_mul_ps(y, z);

    __m128 tmp = _mm_mul_ps(z, half.f);
    y = _mm_sub_ps(y, tmp);

    // base_consts[3, 2, 1, 0] -> l10_2head, l10_2trail, l10_Ehead, l10_Etrail 

    z   = _mm_add_ps(x.f, y);
    z   = _mm_mul_ps(z  , _mm_shuffle_ps(base_consts, base_consts, _MM_SHUFFLE(0,0,0,0)));
    tmp = _mm_mul_ps(y  , _mm_shuffle_ps(base_consts, base_consts, _MM_SHUFFLE(1,1,1,1)));
    z   = _mm_add_ps(z  , tmp);
    tmp = _mm_mul_ps(     _mm_shuffle_ps(base_consts, base_consts, _MM_SHUFFLE(1,1,1,1)), x.f);
    z   = _mm_add_ps(z  , tmp);
    
    tmp = _mm_mul_ps(e.f, _mm_shuffle_ps(base_consts, base_consts, _MM_SHUFFLE(2,2,2,2)));
    z   = _mm_add_ps(z  , tmp);
    tmp = _mm_mul_ps(e.f, _mm_shuffle_ps(base_consts, base_consts, _MM_SHUFFLE(3,3,3,3)));
    dst.f = _mm_add_ps(z, tmp);
}

ISV  log10_A53_unit(const XMM128 &src, XMM128 &dst, const __m128d &ln2_hi_lo, const __m128d &lg1_lg2,
                            const __m128d &lg3_lg4, const __m128d &lg5_lg6  , const XMM128 &lg7_c1_c2)
{
    XMM128 tdst, one, tsrc;
    one.d = _mm_set1_pd(1.0);
    
    __m128i k  = _mm_srai_epi32(_mm_sub_epi32(src.i, one.i), 20);       // k += (hx>>20)-1023;
    __m128i i  = _mm_slli_epi64(_mm_srli_epi64(k, 63), 52);             // i  = ((u_int32_t)k&0x80000000)>>31;
    __m128i hx = _mm_srli_epi64(_mm_slli_epi64(src.i, 12), 12);         // hx &= 0x000fffff;
            hx = _mm_or_si128(hx, _mm_sub_epi64(one.i, i));             // hx = hx|((0x3ff-i)<<20);
             k = _mm_add_epi32(k, _mm_srli_epi32(i, 20));
             k = _mm_shuffle_epi32(k, _MM_SHUFFLE(2, 0, 3, 1));
    __m128d y  = _mm_cvtepi32_pd(k);                                    // y  = (double)(k+i);

    tsrc.i = hx ;                                                       //    SET_HIGH_WORD(x,hx);
    ln_64f_unit(tsrc, tdst, ln2_hi_lo, lg1_lg2, lg3_lg4, lg5_lg6, lg7_c1_c2 ); 

    static const __m128d ivln10     =  CONST_SET1_64F(4.34294481903251816668e-01);
    static const __m128d log10_2hi  =  CONST_SET1_64F(3.01029995663611771306e-01);
    static const __m128d log10_2lo  =  CONST_SET1_64F(3.69423907715893078616e-13);

    __m128d z = _mm_mul_pd(tdst.d, ivln10);                             // z  = y*log10_2lo + ivln10*__ieee754_log(x);
        dst.d = _mm_mul_pd(y, log10_2lo);
            z = _mm_add_pd(z, dst.d);

    dst.d = _mm_mul_pd(y, log10_2hi);
    dst.d = _mm_add_pd(z, dst.d);                                       // return  z+y*log10_2hi;    
}

ISV s10Log10_32s_unit( const __m128i& src, __m128i& dst, 
                       const __m128 &logE_base10xscalex10,
                       const __m128 &ln2_lead, const __m128 &ln2_trail, const __m128 &A1_A2)
{
    XMM128 tdst, tsrc ;

    tsrc.f = _mm_cvtepi32_ps(src);
    CBL_LIBRARY::CBL_Library_Internal::ln_32f_tang_unit(tsrc, tdst ,ln2_lead, ln2_trail, A1_A2);
    tdst.f = _mm_mul_ps(tdst.f, logE_base10xscalex10 );  
    dst    = _mm_cvtps_epi32(tdst.f);
}

ISV s10Log10_32s_special_case( const __m128i &src, __m128i &dst)
{
    // src <=0 => dst = min_32s (0x80000000) or dst

    __m128i zero = _mm_setzero_si128();
    __m128i cmp  = _mm_cmpgt_epi32(src, zero);
    dst = _mm_and_si128(cmp, dst);
    cmp = _mm_cmpeq_epi32(cmp, zero);
    cmp = _mm_slli_epi32(cmp, 31);
    dst = _mm_or_si128(dst, cmp);
}


// SSE2 CALLING FUNCTIONS


template <CH cd>
ISV Ln_8u_sse(const XMM128 *src, XMM128 *dst, const __m128 & scale)
{
    for(int i=0 ; i < cd ; i++) 
        ln_8u_unit(src[i], dst[i], scale);
}

template <CH cd>
ISV Ln_16s_ScaleLT0(const XMM128 *src, XMM128 *dst, 
                    const __m128 & ln2, const __m128i &one_twenty7, int scale)
{
    for(int i=0 ; i < cd ; i++) 
        ln_16s_scaleLT0_unit(src[i], dst[i], ln2, one_twenty7, scale );
}

template <CH cd>
ISV Ln_16s_Scale0(const XMM128 *src, XMM128 *dst, 
                  const __m128 & ln2, const __m128i &one_twenty7, const __m128i &min_16s)
{
    for(int i=0 ; i < cd ; i++) 
        ln_16s_scale0_unit(src[i], dst[i], ln2, one_twenty7, min_16s );
}

template <CH cd>
ISV Ln_16s_Scale1(const XMM128 *src, XMM128 *dst, 
                  const __m128i &twenty_1, const __m128i &one_49,
                  const __m128i &one_097 , const __m128i &eight_104)
{
    for(int i=0 ; i < cd ; i++) 
        ln_16s_scale1_unit(src[i], dst[i], twenty_1, one_49, one_097, eight_104 );
}

template <CH cd>
ISV Ln_16s_Scale2(const XMM128 *src, XMM128 *dst, 
                  const __m128i &eight, const __m128i &four_04, const __m128i &twenty_two_027)
{
    for(int i=0 ; i < cd ; i++) 
        ln_16s_scale2_unit(src[i], dst[i], eight, four_04, twenty_two_027 );
}

template <CH cd>
ISV Ln_16s_ScaleGT2(const XMM128 *src, XMM128 *dst,
                    const __m128i &scale, const __m128i &scale3_const, 
                    const __m128i &scale4_const, const __m128i &four)
{
    for(int i=0 ; i < cd ; i++) 
        ln_16s_scaleGT2_unit(src[i], dst[i], scale, scale3_const, scale4_const, four );
}

template <CH cd>
ISV Ln_32f_tang_sse2(const XMM128 *src, XMM128 *dst, 
                                  const __m128 &ln2_lead, const __m128 &ln2_trail, const __m128 &A1_A2)
{    
    XMM128 tsrc ;
    for(int i=0 ; i < cd ; i++)
    {
        tsrc = src[i];
        CBL_LIBRARY::CBL_Library_Internal::ln_32f_tang_unit(src[i], dst[i] , ln2_lead, ln2_trail, A1_A2);
        ln_32f_special_case(tsrc, dst[i]);
    }
}

//more accurate
template <CH cd>
ISV Ln_32f_sse2(const XMM128 *src, XMM128 *dst, 
                                   const __m128 &hf_sqhalf, const __m128 &const1_const2,
                                   const __m128 &p1_p2, const __m128 &p3,
                                   const __m128 &p4_p5, const __m128 & p6_p7, 
                                   const __m128 &p8_p9)
{    
    XMM128 tsrc ;
    for(int i=0 ; i < cd ; i++)
    {
        tsrc = src[i];
        ln_32f_unit(src[i], dst[i], hf_sqhalf, const1_const2, p1_p2, p3, p4_p5, p6_p7, p8_p9);
        ln_32f_special_case(tsrc, dst[i]);    
    }
}

ISV Ln_32s_sse2(const XMM128 &src, XMM128 &dst, const __m128 &scale, const __m128 &ln_2, const __m128i &one_twenty7 )
{    
    XMM128 tsrc, tdst ;

    tsrc.f = _mm_cvtepi32_ps(src.i);
    ln_lowAccuracy_unit(tsrc, tdst, ln_2, one_twenty7);
    tdst.f = _mm_mul_ps(tdst.f, scale); //scale
    tdst.i = _mm_cvtps_epi32(tdst.f);
    ln_32s_special_case(src.i, tdst.i);
    dst.i = tdst.i ;
}

ISV Ln_32s_16s_sse2(const XMM128 &src, XMM128 &dst, const __m128 &scale, const __m128 &ln_2, const __m128i &one_twenty7 )
{    
    XMM128 tsrc ;

    tsrc.f = _mm_cvtepi32_ps(src.i);
    ln_lowAccuracy_unit(tsrc, dst, ln_2, one_twenty7);
    dst.f = _mm_mul_ps(dst.f, scale);    
    dst.i = _mm_cvtps_epi32(dst.f);
    ln_32s_special_case(src.i, dst.i);
    dst.i = _mm_packs_epi32(dst.i, dst.i); 
}

ISV Ln_64f_sse2(const XMM128 &src, XMM128 &dst, 
                const __m128d &ln2_hi_lo, const __m128d &lg1_lg2,
                const __m128d &lg3_lg4, const __m128d &lg5_lg6, const XMM128 &lg7_c1_c2)
{    
    XMM128 tsrc ;
    tsrc.d = src.d ; //for in-place backup
    ln_64f_unit(src, dst, ln2_hi_lo, lg1_lg2, lg3_lg4, lg5_lg6, lg7_c1_c2 );  
    ln_64f_special_case(tsrc, dst);    
}

ISV Ln_A21_sse2(const XMM128 &src, XMM128 &dst, 
                                   const __m128 &ln2_lead, const __m128 &ln2_trail,
                                   const __m128i &exponent_127, const __m128 &A1_A2)
{
    ln_A21_unit(src, dst, ln2_lead, ln2_trail, exponent_127, A1_A2);
    CBL_LIBRARY::CBL_Library_Internal::ln_A11_special_case(src, dst);
}

    // Log

ISV Log10_A21_sse2(const XMM128 &src, XMM128 &dst,
                                       const __m128 &half_sqhalf, 
                                       const __m128 &const1_const2,
                                       const __m128 &p1_p2, const __m128 &p3,
                                       const __m128 &p4_p5, const __m128 &p6_p7,
                                       const __m128 &p8_p9)
{
    ln_32f_unit(src, dst, half_sqhalf, const1_const2, p1_p2, p3, p4_p5, p6_p7, p8_p9);
    CBL_LIBRARY::CBL_Library_Internal::ln_A11_special_case(src, dst);
    static const __m128 logE_base10 = CONST_SET1_32F(0.43429448190325176F);
    dst.f  = _mm_mul_ps(dst.f, logE_base10);
}

ISV Log10_A24_sse2(const XMM128 &src, XMM128 &dst,
                                       const __m128 &half_sqhalf, 
                                       const __m128 &const1_const2,
                                       const __m128 &p1_p2, const __m128 &p3,
                                       const __m128 &p4_p5, const __m128 &p6_p7,
                                       const __m128 &p8_p9)
{
    XMM128 tsrc, tdst ;

    __m128i hx = _mm_srli_epi32(_mm_slli_epi32(src.i, 1), 1);
    __m128i one_27 = _mm_srli_epi32(_mm_cmpeq_epi32(src.i, src.i), 25);         //127 i.e, 7F
    __m128i k  = _mm_srli_epi32(hx, 23);
            k  = _mm_sub_epi32(k, one_27);                                      //k = (hx>>23)-127;
    __m128i i  = _mm_srli_epi32(k, 31);
            hx = _mm_srli_epi32(_mm_slli_epi32(hx, 9), 9);
    __m128  y  = _mm_cvtepi32_ps(_mm_add_epi32(k, i));
            i  = _mm_slli_epi32(_mm_sub_epi32(one_27, i), 23);
            hx = _mm_or_si128(hx, i);
        tsrc.i = hx ;                                                           // _mm_cvtepi32_ps(hx);
    
    ln_32f_unit(tsrc, tdst, half_sqhalf, const1_const2, p1_p2, p3, p4_p5, p6_p7, p8_p9);

    static const __m128 ivln10     =  CONST_SET1_32F(4.3429449201e-01F);
    static const __m128 log10_2hi  =  CONST_SET1_32F(3.0102920532e-01F);
    static const __m128 log10_2lo  =  CONST_SET1_32F(7.9034151668e-07F);

    tdst.f = _mm_mul_ps(tdst.f, ivln10);
    dst.f  = _mm_mul_ps(y, log10_2lo);
    dst.f  = _mm_add_ps(dst.f, tdst.f);
    tdst.f = _mm_mul_ps(y, log10_2hi);
    dst.f  = _mm_add_ps(dst.f, tdst.f );

    CBL_LIBRARY::CBL_Library_Internal::ln_A11_special_case(src, dst);

}

ISV Log10_A50_sse2(const XMM128 &src, XMM128 &dst, 
                const __m128d &ln2_hi_lo, const __m128d &lg1_lg2,
                const __m128d &lg3_lg4  , const __m128d &lg5_lg6, const XMM128 &lg7_c1_c2)
{
    Ln_64f_sse2(src, dst, ln2_hi_lo, lg1_lg2, lg3_lg4, lg5_lg6, lg7_c1_c2 ); 
    static const __m128d logE_base10 = CONST_SET1_64F(0.43429448190325182);
    dst.d = _mm_mul_pd(dst.d, logE_base10);
}


ISV  Log10_A53_sse2(const XMM128 &src, XMM128 &dst, const __m128d &ln2_hi_lo, const __m128d &lg1_lg2,
                                                    const __m128d &lg3_lg4  , const __m128d &lg5_lg6, const XMM128 &lg7_c1_c2)
{
    log10_A53_unit(src, dst, ln2_hi_lo, lg1_lg2, lg3_lg4, lg5_lg6, lg7_c1_c2);  
    ln_64f_special_case(src, dst);    
}

ISV TenLog10_32s_sse2(const XMM128 &src, XMM128 &dst, 
                      const __m128 &logE_base10xscalex10,
                      const __m128 &ln2_lead, const __m128 &ln2_trail, const __m128 &A1_A2)
{
    __m128i tsrc_i  = src.i ;
    s10Log10_32s_unit(src.i, dst.i, logE_base10xscalex10, ln2_lead, ln2_trail, A1_A2 );
    s10Log10_32s_special_case(tsrc_i, dst.i);
}

    // STRUCTURES of Ln_8u, _16s, _32s, _32f,_64f for 1 and 2 buffers.

  template<CH cd>
  struct Ln_8u_B1 : public fe1< Fw8u, cd >
  {   
        FE_SSE2_REF
     
        __m128 m_scale ;
        float scale;

         Ln_8u_B1(int scaleFactor)
         {
             scale = calcScale(scaleFactor); 
         }

         IV SSE2_Init()
         {
             ln_8u_setup(scale, m_scale);
         }
         IV SSE2( RegFile & r ) const
         {
             Ln_8u_sse<cd>(r.dst, r.dst, m_scale);
         }
         IV REFR( Fw8u *sd) const
         { 
             Ln_8u_ref<cd>( sd, sd, scale);
         }
    } ;

    template<CH cd>
    struct Ln_8u_B2 : public fe2< Fw8u, cd, Fw8u, cd >
    {   
        FE_SSE2_REF    

        __m128 m_scale ;
        float scale;

         Ln_8u_B2(int scaleFactor)
         {
             scale = calcScale(scaleFactor);
         }

         IV SSE2_Init()
         {
              ln_8u_setup(scale, m_scale);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_8u_sse<cd>(r.src1, r.dst, m_scale);
         }      
         IV REFR( const Fw8u *s, Fw8u *d) const
         {         
              Ln_8u_ref<cd >(s, d, scale);
         }      
    } ;


    // 16s Non-inplace
    template<CH cd>
    struct Ln_16s_B1 : public fe1< Fw16s, cd >
    { 
        FE_SSE2_REF
        float scale;

         Ln_16s_B1(int scaleFactor)
         {
             scale = calcScale(scaleFactor);
         }
         IV SSE2_Init() {}
         IV SSE2( RegFile & r ) const {}
         IV REFR( Fw16s *srcDst) const
         {         
              Ln_int_ref<Fw16s, Fw16s, cd >(srcDst, srcDst, scale);
         }    
    };

    template<CH cd>
    struct Ln_16s_ScaleLT0_B1 : public Ln_16s_B1< cd >
    {       
        FE_SSE2_REF
        
        int sf;
        XMM128 ln2;
        XMM128 exponent_127 ;

        Ln_16s_ScaleLT0_B1(int scaleFactor) : Ln_16s_B1<cd>(scaleFactor){sf = scaleFactor;}
             
         IV SSE2_Init()
         {
             ln_16s_scaleLT0_setup(ln2.f, exponent_127.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_16s_ScaleLT0<cd>(r.dst, r.dst, ln2.f, exponent_127.i, sf);
         }
    } ;

    template<CH cd>
    struct Ln_16s_Scale0_B1 : public Ln_16s_B1< cd >
    {       
        FE_SSE2_REF
        
        XMM128 ln2,exponent_127, min_16s ; 
            
        Ln_16s_Scale0_B1(int scaleFactor) : Ln_16s_B1<cd>(scaleFactor){ }
             
         IV SSE2_Init()
         {
             ln_16s_scale0_setup(ln2.f, exponent_127.i, min_16s.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_16s_Scale0<cd>(r.dst, r.dst, ln2.f, exponent_127.i, min_16s.i);
         }     
    } ;
    template<CH cd>
    struct Ln_16s_Scale1_B1 : public Ln_16s_B1< cd >
    {       
        FE_SSE2_REF
        
        XMM128 twenty_1, one_49, one_097, eight_104 ;

        Ln_16s_Scale1_B1(int scaleFactor) : Ln_16s_B1<cd>(scaleFactor){ }
             
         IV SSE2_Init()
         {
             ln_16s_scale1_setup(twenty_1.i, one_49.i, one_097.i, eight_104.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_16s_Scale1<cd>(r.dst, r.dst, twenty_1.i, one_49.i, one_097.i, eight_104.i);
         }    
    } ;
    
    template<CH cd>
    struct Ln_16s_Scale2_B1 : public Ln_16s_B1< cd >
    {       
        FE_SSE2_REF
        
        XMM128 eight, four_04, twenty_two_027 ;
    
        Ln_16s_Scale2_B1(int scaleFactor) : Ln_16s_B1<cd>(scaleFactor){ }
             
         IV SSE2_Init()
         {
             ln_16s_scale2_setup(eight.i, four_04.i, twenty_two_027.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_16s_Scale2<cd>(r.dst, r.dst, eight.i, four_04.i, twenty_two_027.i);
         }       
    } ;
    
    template<CH cd>
    struct Ln_16s_ScaleGT2_B1 : public Ln_16s_B1< cd >
    {       
        FE_SSE2_REF  
        int sf;        
        XMM128 m_scale, scale3_const, scale4_const, four ;

        Ln_16s_ScaleGT2_B1(int scaleFactor) : Ln_16s_B1<cd>(scaleFactor){ sf = scaleFactor;}
             
         IV SSE2_Init()
         {
             ln_16s_scaleGT2_setup(sf, m_scale.i, scale3_const.i, scale4_const.i, four.i );
         }
         IV SSE2( RegFile & r ) const 
         {      
             Ln_16s_ScaleGT2<cd>(r.dst, r.dst, m_scale.i, scale3_const.i, scale4_const.i, four.i);
         }     
    } ;

    // 16s inplace
    template<CH cd>
    struct Ln_16s_B2 : public fe2< Fw16s, cd, Fw16s, cd >
    {  
        FE_SSE2_REF
        float scale;

         Ln_16s_B2(int scaleFactor){
             scale = calcScale(scaleFactor);
         }
         IV SSE2_Init() {}
         IV SSE2( RegFile & r ) const {}
         IV REFR( const Fw16s *src, Fw16s *dst) const
         {         
              Ln_int_ref<Fw16s, Fw16s, cd >(src, dst, scale);
         }
    };

    template<CH cd>
    struct Ln_16s_ScaleLT0_B2 : public Ln_16s_B2< cd >
    {       
        FE_SSE2_REF
        
        int sf;
        XMM128 ln2;
        XMM128 exponent_127 ;

        Ln_16s_ScaleLT0_B2(int scaleFactor) : Ln_16s_B2<cd>(scaleFactor){sf = scaleFactor;}
             
         IV SSE2_Init()
         {
             ln_16s_scaleLT0_setup(ln2.f, exponent_127.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_16s_ScaleLT0<cd>(r.src1, r.dst, ln2.f, exponent_127.i, sf);
         }      
    } ;

    template<CH cd>
    struct Ln_16s_Scale0_B2 : public Ln_16s_B2< cd >
    {       
        FE_SSE2_REF
        
        XMM128 ln2,exponent_127, min_16s ; 
            
        Ln_16s_Scale0_B2(int scaleFactor) : Ln_16s_B2<cd>(scaleFactor){ }
             
         IV SSE2_Init()
         {
             ln_16s_scale0_setup(ln2.f, exponent_127.i, min_16s.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Ln_16s_Scale0<cd>(r.src1, r.dst, ln2.f, exponent_127.i, min_16s.i);
         }        
    } ;
    template<CH cd>
    struct Ln_16s_Scale1_B2 : public Ln_16s_B2< cd >
    {       
        FE_SSE2_REF
        
        XMM128 twenty_1, one_49, one_097, eight_104 ;

        Ln_16s_Scale1_B2(int scaleFactor) : Ln_16s_B2<cd>(scaleFactor){ }
             
        IV SSE2_Init()
        {
            ln_16s_scale1_setup(twenty_1.i, one_49.i, one_097.i, eight_104.i);
        }
        IV SSE2( RegFile & r ) const
        {      
            Ln_16s_Scale1<cd>(r.src1, r.dst, twenty_1.i, one_49.i, one_097.i, eight_104.i);
        }
    } ;
    
    template<CH cd>
    struct Ln_16s_Scale2_B2 : public Ln_16s_B2< cd >
    {       
        FE_SSE2_REF
        
        XMM128 eight, four_04, twenty_two_027 ;
    
        Ln_16s_Scale2_B2(int scaleFactor) : Ln_16s_B2<cd>(scaleFactor){ }
             
        IV SSE2_Init()
        {
            ln_16s_scale2_setup(eight.i, four_04.i, twenty_two_027.i);
        }
        IV SSE2( RegFile & r ) const
        {      
            Ln_16s_Scale2<cd>(r.src1, r.dst, eight.i, four_04.i, twenty_two_027.i);
        }
    } ;
    
    template<CH cd>
    struct Ln_16s_ScaleGT2_B2 : public Ln_16s_B2< cd >
    {       
        FE_SSE2_REF  
        int sf;        
        XMM128 m_scale, scale3_const, scale4_const, four ;

        Ln_16s_ScaleGT2_B2(int scaleFactor) : Ln_16s_B2<cd>(scaleFactor){ sf = scaleFactor;}
             
        IV SSE2_Init()
        {
            ln_16s_scaleGT2_setup(sf, m_scale.i, scale3_const.i, scale4_const.i, four.i );
        }
        IV SSE2( RegFile & r ) const
        {      
            Ln_16s_ScaleGT2<cd>(r.src1, r.dst, m_scale.i, scale3_const.i, scale4_const.i, four.i);
        }
    } ;

    // 32f

    template<CH cd>
    struct Ln_32f_tang_B1 : public fe1< Fw32f, cd >          
    {       
        FE_SSE2_REF  
        XMM128 ln2_lead, ln2_trail, A1_A2 ;

        Ln_32f_tang_B1(){}
             
        IV SSE2_Init()
        {
            CBL_LIBRARY::CBL_Library_Internal::ln_32f_tang_setup(ln2_lead.f, ln2_trail.f, A1_A2.f);
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_32f_tang_sse2<cd>(r.dst, r.dst, ln2_lead.f, ln2_trail.f, A1_A2.f);         
        }          
        IV REFR( Fw32f *srcDst) const                     
        {         
              Ln_float_ref<Fw32f, Fw32f, cd>( srcDst, srcDst);
        }        
    } ;

    template<CH cd>
    struct Ln_32f_tang_B2 : public fe2< Fw32f, cd, Fw32f, cd >          
    {       
        FE_SSE2_REF  
        XMM128 ln2_lead, ln2_trail, A1_A2 ;

        Ln_32f_tang_B2(){}
             
        IV SSE2_Init()
        {
            CBL_LIBRARY::CBL_Library_Internal::ln_32f_tang_setup(ln2_lead.f, ln2_trail.f, A1_A2.f);        
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_32f_tang_sse2<cd>(r.src1, r.dst, ln2_lead.f, ln2_trail.f, A1_A2.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
             Ln_float_ref<Fw32f, Fw32f, cd>( src, dst);
        }  
    } ;

    // 32s
    template<CH cd>
    struct Ln_32s_B1 : public fe1< Fw32s, cd >          
    {       
        FE_SSE2_REF  
        XMM128 m_scale, ln_2, exponent_127 ;
        float scale ;

        Ln_32s_B1(int scaleFactor ){ 
            scale = calcScale(scaleFactor);
        }
             
        IV SSE2_Init()
        {
            ln_32s_setup(scale, m_scale.f, ln_2.f, exponent_127.i);        
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_32s_sse2(r.dst[0], r.dst[0], m_scale.f, ln_2.f, exponent_127.i);         
        }          
        IV REFR( Fw32s *srcDst) const                     
        {         
              Ln_int_ref<Fw32s, Fw32s, cd>( srcDst, srcDst, scale);
        }        
    } ;

    template<CH cd>
    struct Ln_32s_B2 : public fe2< Fw32s, cd, Fw32s, cd  >          
    {       
        FE_SSE2_REF  
        XMM128 m_scale, ln_2, exponent_127 ;
        float scale ;

        Ln_32s_B2(int scaleFactor ){ 
            scale = calcScale(scaleFactor);
        }
             
        IV SSE2_Init()
        {
            ln_32s_setup(scale, m_scale.f, ln_2.f, exponent_127.i);        
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_32s_sse2(r.src1[0], r.dst[0], m_scale.f, ln_2.f, exponent_127.i);         
        }          
        IV REFR( const Fw32s *src, Fw32s *dst) const                     
        {         
              Ln_int_ref<Fw32s, Fw32s, cd>( src, dst, scale);
        }        
    } ;

    template<CH cd>
    struct Ln_32s_16s_B2 : public fe2< Fw32s, cd , Fw16s, cd >          
    {       
        FE_SSE2_REF  
        XMM128 m_scale, ln_2, exponent_127 ;
        float scale ;

        Ln_32s_16s_B2(int scaleFactor ){ 
            scale = calcScale(scaleFactor);
        }
             
        IV SSE2_Init()
        {
            ln_32s_setup(scale, m_scale.f, ln_2.f, exponent_127.i);        
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_32s_16s_sse2(r.src1[0], r.dst[0], m_scale.f, ln_2.f, exponent_127.i);         
        }          
        IV REFR( const Fw32s *src, Fw16s *dst) const                     
        {         
              Ln_int_ref< Fw32s, Fw16s, cd>( src, dst, scale);
        }        
    } ;

    template<CH cd>
    struct Ln_64f_B1 : public fe1< Fw64f, cd>
    {
        FE_SSE2_REF 
        Ln_64f_B1() {}
        XMM128 ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts ;


        IV SSE2_Init()
        {
            ln_64f_setup(ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_64f_sse2(r.dst[0], r.dst[0], 
                 ln2_hi_lo.d, ln_const_1_2.d, ln_const_3_4.d, ln_const_5_6.d, ln_const_7_hexConsts);         
        }          
        IV REFR( Fw64f *srcDst) const                     
        {         
              Ln_float_ref< Fw64f, Fw64f, cd>( srcDst, srcDst);
        }        
    };

    template<CH cd>
    struct Ln_64f_B2 : public fe2< Fw64f, cd, Fw64f, cd>
    {
        FE_SSE2_REF 
        Ln_64f_B2() {}

        XMM128 ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts ;


        IV SSE2_Init()
        {
            ln_64f_setup(ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_64f_sse2(r.src1[0], r.dst[0], 
                 ln2_hi_lo.d, ln_const_1_2.d, ln_const_3_4.d, ln_const_5_6.d, ln_const_7_hexConsts);         
        }          
        IV REFR( const Fw64f *src, Fw64f *dst) const                     
        {         
              Ln_float_ref< Fw64f, Fw64f, cd>( src, dst);
        }        
    };
    
    template<CH cd>
    struct Ln_64f_32f_B2 : public fe2< Fw64f, cd, Fw32f, cd>
    {
        FE_SSE2_REF 
        Ln_64f_32f_B2() {}

        XMM128 ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts ;


        IV SSE2_Init()
        {
            ln_64f_setup(ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_64f_sse2(r.src1[0], r.dst[0], 
                ln2_hi_lo.d, ln_const_1_2.d, ln_const_3_4.d, ln_const_5_6.d, ln_const_7_hexConsts);         
            r.dst[0].f = _mm_cvtpd_ps(r.dst[0].d);
        }          
        IV REFR( const Fw64f *src, Fw32f *dst) const                     
        {         
              Ln_float_ref< Fw64f, Fw32f, cd>( src, dst);
        }        
    };

    // Fixed Accuracy stuff

    struct Ln_32f_A11 : public fe2< Fw32f, C1, Fw32f, C1 >          
    {       
        FE_SSE2_REF  
        XMM128 half_sqhalf, p5, p6_p7, p8_p9, const1_const2;

        Ln_32f_A11(){}
         
        IV SSE2_Init()
        {
            CBL_LIBRARY::CBL_Library_Internal::ln_A11_setup(half_sqhalf.f, p5.f, p6_p7.f, p8_p9.f, const1_const2.f);
        }
        IV SSE2( RegFile & r ) const                     
        {      
            CBL_LIBRARY::CBL_Library_Internal::Ln_A11_sse2(r.src1[0], r.dst[0], half_sqhalf.f, p5.f, p6_p7.f, p8_p9.f, const1_const2.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
              Ln_float_ref<Fw32f, Fw32f, C1>( src, dst);
        }  
    } ;

    struct Ln_32f_A21 : public fe2< Fw32f, C1, Fw32f, C1 >          
    {       
        FE_SSE2_REF  
        XMM128 ln2_lead, ln2_trail, exponent_127, A1_A2;

        Ln_32f_A21(){}
        
        IV SSE2_Init()
        {
            ln_A21_setup(ln2_lead.f, ln2_trail.f, exponent_127.i, A1_A2.f);
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_A21_sse2(r.src1[0], r.dst[0], ln2_lead.f, ln2_trail.f, exponent_127.i, A1_A2.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
              Ln_float_ref<Fw32f, Fw32f, C1>( src, dst);
        }  
    } ;

    template <CH cd>
    struct Ln_32f : public fe2< Fw32f, cd, Fw32f, cd >
    {       
        FE_SSE2_REF  
        XMM128 hf_sqhalf, const1_const2, p1_p2, p3, p4_p5,  p6_p7, p8_p9;

        Ln_32f(){}
        
        IV SSE2_Init()
        {
            ln_32f_setup(hf_sqhalf.f, const1_const2.f, p1_p2.f, p3.f, p4_p5.f,  p6_p7.f, p8_p9.f);
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Ln_32f_sse2<cd>(r.src1, r.dst, hf_sqhalf.f, const1_const2.f,
                p1_p2.f, p3.f, p4_p5.f,  p6_p7.f, p8_p9.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
            Ln_float_ref<Fw32f, Fw32f, cd>( src, dst);
        }  
    } ;

    // Log10

    struct Log10_A11 : public fe2< Fw32f, C1, Fw32f, C1 >
    {       
        FE_SSE2_REF  
        XMM128 hf_sqhalf, p5, p6_p7, p8_p9, const1_const2;

        Log10_A11(){}
        
        IV SSE2_Init()
        {
            CBL_LIBRARY::CBL_Library_Internal::ln_A11_setup(hf_sqhalf.f, p5.f, p6_p7.f, p8_p9.f, const1_const2.f);
        }
        IV SSE2( RegFile & r ) const                     
        {      
            CBL_LIBRARY::CBL_Library_Internal::Log10_A11_sse2(r.src1[0], r.dst[0], hf_sqhalf.f, p5.f, p6_p7.f, p8_p9.f, const1_const2.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
            Log10_float_ref<Fw32f, Fw32f, C1>( src, dst);
        }  
    } ;

    struct Log10_A21 : public fe2< Fw32f, C1, Fw32f, C1 >
    {       
        FE_SSE2_REF  
        XMM128 hf_sqhalf, const1_const2, p1_p2, p3, p4_p5,  p6_p7, p8_p9;

        Log10_A21(){}
        
        IV SSE2_Init()
        {
            ln_32f_setup(hf_sqhalf.f, const1_const2.f, p1_p2.f, p3.f, p4_p5.f, p6_p7.f, p8_p9.f );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Log10_A21_sse2(r.src1[0], r.dst[0], hf_sqhalf.f, const1_const2.f, p1_p2.f, p3.f, p4_p5.f, p6_p7.f, p8_p9.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
            Log10_float_ref<Fw32f, Fw32f, C1>( src, dst);
        }  
    } ;

    struct Log10_A24 : public fe2< Fw32f, C1, Fw32f, C1 >
    {       
        FE_SSE2_REF  
        XMM128 hf_sqhalf, const1_const2, p1_p2, p3, p4_p5,  p6_p7, p8_p9;

        Log10_A24(){}
        
        IV SSE2_Init()
        {
            ln_32f_setup(hf_sqhalf.f, const1_const2.f, p1_p2.f, p3.f, p4_p5.f, p6_p7.f, p8_p9.f );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Log10_A24_sse2(r.src1[0], r.dst[0], hf_sqhalf.f, const1_const2.f, p1_p2.f, p3.f, p4_p5.f, p6_p7.f, p8_p9.f);         
        }  
        IV REFR( const Fw32f *src, Fw32f *dst) const                     
        {         
            Log10_float_ref<Fw32f, Fw32f, C1>( src, dst);
        }  
    } ;

    struct Log10_A50 : public fe2< Fw64f, C1, Fw64f, C1>
    {
        FE_SSE2_REF 
        Log10_A50() {}

        XMM128 ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts ;

        IV SSE2_Init()
        {
            ln_64f_setup(ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Log10_A50_sse2(r.src1[0], r.dst[0], 
                 ln2_hi_lo.d, ln_const_1_2.d, ln_const_3_4.d, ln_const_5_6.d, ln_const_7_hexConsts);         
        }          
        IV REFR( const Fw64f *src, Fw64f *dst) const                     
        {         
              Log10_float_ref< Fw64f, Fw64f, C1>( src, dst);
        }        
    };
 
    struct Log10_A53 : public fe2< Fw64f, C1, Fw64f, C1>
    {
        FE_SSE2_REF 
        Log10_A53() {}

        XMM128 ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts ;

        IV SSE2_Init()
        {
            ln_64f_setup(ln2_hi_lo, ln_const_1_2, ln_const_3_4, ln_const_5_6, ln_const_7_hexConsts );
        }
        IV SSE2( RegFile & r ) const                     
        {      
            Log10_A53_sse2(r.src1[0], r.dst[0], 
                 ln2_hi_lo.d, ln_const_1_2.d, ln_const_3_4.d, ln_const_5_6.d, ln_const_7_hexConsts);         
        }          
        IV REFR( const Fw64f *src, Fw64f *dst) const                     
        {         
              Log10_float_ref< Fw64f, Fw64f, C1>( src, dst);
        }        
    };


    struct TenLog10_32s_B1 : public fe1< Fw32s, C1 >          
    {       
        FE_SSE2_REF 
        XMM128  logE_base10xscalex10, ln2_lead, ln2_trail, A1_A2 ;
        float scale ;

        TenLog10_32s_B1(int scaleFactor )
        { 
            scale = calcScale(scaleFactor);
        }
            
        IV SSE2_Init()
        {
            tenLog10_32s_setup(scale, logE_base10xscalex10.f, ln2_lead.f, ln2_trail.f, A1_A2.f);        
        }
        IV SSE2( RegFile & r ) const                     
        {      
            TenLog10_32s_sse2(r.dst[0], r.dst[0], logE_base10xscalex10.f, ln2_lead.f, ln2_trail.f, A1_A2.f);         
        }          
        IV REFR( Fw32s *srcDst) const                     
        {         
             Log10_32s_ref( srcDst[0], srcDst[0], scale);
        }        
    } ;

    struct TenLog10_32s_B2 : public fe2< Fw32s, C1, Fw32s, C1  >          
    {       
        FE_SSE2_REF 
        XMM128  logE_base10xscalex10, ln2_lead, ln2_trail, A1_A2 ;
        float scale ;

        TenLog10_32s_B2(int scaleFactor )
        { 
            scale = calcScale(scaleFactor);
        }
            
        IV SSE2_Init()
        {
            tenLog10_32s_setup(scale, logE_base10xscalex10.f, ln2_lead.f, ln2_trail.f, A1_A2.f);        
        }
        IV SSE2( RegFile & r ) const                     
        {      
            TenLog10_32s_sse2(r.src1[0], r.dst[0], logE_base10xscalex10.f, ln2_lead.f, ln2_trail.f, A1_A2.f);         
        }          
        IV REFR( const Fw32s *src, Fw32s *dst) const                     
        {         
             Log10_32s_ref( src[0], dst[0], scale);
        }        
    } ;


} // namespace OPT_LEVEL

#endif // _LN_H_

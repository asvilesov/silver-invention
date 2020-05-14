Alexander Vilesov
Email: asvilesov@gmail.com

Project: DNA Sequencing and Global Pairwise Sequencing

Instructions:

1) cd build
2) cmake ..
3) make
4) cd ../bin
5) ./main {args}

Two modes:

A. If one arg is provided to a fasta file, a AMINO.txt file will be generated which sequences the DNA sequence and creates a histogram of the amino acids
For example: 
1) ./main ./main ../input/TAS2R16_Homo_sapiens.fasta
2) cat amino.txt

gi|228480242:5001-5996 Homo sapiens taste receptor, type 2, member 16 (TAS2R16), RefSeqGene on chromosome 7
Total amino acids produced: 291
(A) Alanine: 12
(C) Cysteine: 8
(D) Aspartic acid: 3
(E) Glutamic acid: 6
(F) Phenylalanine: 24
(G) Glycine: 7
(H) Histidine: 9
(I) Isoleucine: 33
(K) Lysine: 8
(L) Leucine: 46
(M) Methionine: 11
(N) Asparagine: 11
(P) Proline: 8
(Q) Glutamine: 11
(R) Arginine: 12
(S) Serine: 23
(T) Threonine: 23
(V) Valine: 18
(W) Tryptophan: 10
(Y) Tyrosine: 8

B. If two args are provided, the two fasta files containing DNA sequences will undergo global pairwise sequencing using the Needleman-Wunsch Algorithm. The resulting matching results will be placed in "match.result"
For example:
1) ./main ../input/Zaire_ebolavirus.fasta ../input/Reston_ebolavirus.fasta
2) cat match.result

A: gi|10313991|ref|NC_002549.1| Zaire ebolavirus isolate Ebola virus/H.sapiens-tc/COD/1976/Yambuku-Mayinga, complete genome
B: gi|22789222|ref|NC_004161.1| Reston ebolavirus isolate Reston virus/M.fascicularis-tc/USA/1989/Philippines89-Pennsylvania, complete genome
Score: 6958

CGGACACACAAAAAGAAAGAAGAATTTTTAGGA_TCTTTTGTGTGCGAATAACTATGAGGAAGATTAATA
|||||||||||||||||| |||  |||||| || | |||||||||||| ||||||||||||||||||| |
CGGACACACAAAAAGAAAAAAGGTTTTTTAAGACT_TTTTGTGTGCGAGTAACTATGAGGAAGATTAACA

ATTTTCCTCTCA_TTGAA_ATTTATA_TCGGAATTTAAATTGAAATTGT__TAC__TG_TAATC____AC
 |||| | |||| || || || || | | | |||| | ||||| ||| |  | |  || || ||    ||
GTTTT_C_CTCAGTTTAAGATATACACT_GAAATTGAGATTGAGATTCTCCT_CTTTGCTATTCTGTAAC

___ACCTGGTTTGTTTCAGAGCCACATCACAAAGATAGA_GAACAACCTAGGTC_TCCGAAGGGA_GCAA
    ||||| ||||  || |   | ||||     ||  |  | |||  ||   | | | ||   | |  |
TTTCCCTGG_TTGTGACA_ATTGA_ATCAGTTTTATCTATTACCAA_TTA__CCAT_C_AA__CATGGTA

GGGCATCAGTGTGCTCAGTTGAAAATCCCTTGTCAACACCTAGGTCTTATCACATCACAAGTTC_CACCT
 | | | |  ||| ||  |||   | | | | ||  || || ||| || || | | |  || ||  |  |
TGTC_T_A__GTGATC__TTG_GGA_CTC_T_TCTTCATCT_GGT_TTTTC_C_T_A_GAGCTCTGA_AT

CAGACTCTGC_AG__GGTGATCC_AAC_AACC__T_TAATAGAAACATTATTGTTAAAGG_ACAGCATTA
|  | | ||| ||  | | |||| ||| | ||  | |  | |||| | ||     |||||  |  |  | 
C_CATTTTGCGAGAAGTTCATCCAAACGACCCAGTGT_CT_GAAA_A_TA___CAAAAGGTTC__CCCT_

GTTCACAGTCAA__ACAAGCAAGATTGAGAATTAACCTTG_GTTTTGAACTT_GAA_CACT_TAG_G___
 ||| | |||||    |||   | ||  |  || |  ||| || | ||  ||  || | ||  || |   
_TTC_C_GTCAAGTTTAAG__GGGTT__GTTTTGA__TTGTGTGTAGATTTTATAATC_CTAGAGTGCCA

_GGA_TTGAAGAT_TCAACAACCCTAAAGCTTGGGGTAA_AACATTGG__A_AA_TAGTT___A_AA_A_
 ||| |||  | | ||| | |   |   | || ||| || | ||  ||  | || | |||   | || | 
AGGAGTTG_CG_TGTCATC_A___T__TGATT_GGG_AAGATCA_AGGAAACAATTTGTTCCAATAATAT

_G_ACAAATTG_CTCGGAA_TC__ACAA____AATTCCGAGTATGGATTC_TCGTCCTCAGAA_AATCTG
 | |||  ||| ||   || ||  ||||    || | ||| |||||| || | |  | ||||| ||||||
CGTACATCTTGACT___AAGTCGAACAAGGGGAAGT_CGA_TATGGA_TCGTGGGAC_CAGAAGAATCTG

GATGGCGCCGAGTCTCA_CTGA_ATCTGACATGGATTACCACAAGATCTTGACAGCAGGTCTGT_CCGTT
| || |||  | ||  |  ||| | ||||  | ||||| || || || |||||||| || || | | |||
GGTGTCGCAAAATC_AAGGTGATA_CTGATTTAGATTATCATAAAATTTTGACAGCTGGCCT_TACTGTT

CAACAGGGGATTGTTC_GGCA_AAGAGTCATCCCAGTGTATCAAGTAAACAATCTT_GAAGAAATTTGCC
|||||||| |||| || |||| || | | ||  | || ||||  ||  | || ||| || |  || || |
CAACAGGGAATTG_TCAGGCAGAA_AATAATTTCTGTATATCTTGTTGATAA_CTTGGAGGCTATGTGTC

...

...

...

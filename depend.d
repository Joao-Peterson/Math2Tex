main.o: main.c modules/worksheet_parser/worksheet_parser.h \
 modules/worksheet_parser/../xml_parser/xml_parser.h \
 modules/worksheet_parser/../logger/logger.h \
 modules/worksheet_parser/../data_structures/data_structures.h \
 c:\mathcad_parser\parser\modules\xml_parser\xml_parser.h \
 c:\mathcad_parser\parser\modules\logger\logger.h \
 modules/results_parser/results_parser.h \
 modules/results_parser/../xml_parser/xml_parser.h \
 modules/results_parser/../logger/logger.h \
 modules/results_parser/../data_structures/data_structures.h
data_structures.o: modules/data_structures/data_structures.c \
 modules/data_structures/data_structures.h \
 modules/data_structures/../xml_parser/xml_parser.h \
 modules/data_structures/../logger/logger.h
logger.o: modules/logger/logger.c modules/logger/logger.h
results_parser.o: modules/results_parser/results_parser.c \
 modules/results_parser/results_parser.h \
 modules/results_parser/../xml_parser/xml_parser.h \
 modules/results_parser/../logger/logger.h \
 modules/results_parser/../data_structures/data_structures.h \
 c:\mathcad_parser\parser\modules\xml_parser\xml_parser.h \
 c:\mathcad_parser\parser\modules\logger\logger.h
worksheet_parser.o: modules/worksheet_parser/worksheet_parser.c \
 modules/worksheet_parser/worksheet_parser.h \
 modules/worksheet_parser/../xml_parser/xml_parser.h \
 modules/worksheet_parser/../logger/logger.h \
 modules/worksheet_parser/../data_structures/data_structures.h \
 c:\mathcad_parser\parser\modules\xml_parser\xml_parser.h \
 c:\mathcad_parser\parser\modules\logger\logger.h
xml_parser.o: modules/xml_parser/xml_parser.c \
 modules/xml_parser/xml_parser.h

install(TARGETS synq DESTINATION bin)
install(DIRECTORY plugins/ DESTINATION share/synq/plugins FILES_MATCHING PATTERN "*.json")

set(CPACK_GENERATOR "ZIP")
include(CPack)

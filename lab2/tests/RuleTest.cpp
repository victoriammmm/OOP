#include <gtest/gtest.h>
#include "Rule.h"

TEST(RuleTest, ConstructorValidRules) {
    EXPECT_NO_THROW(Rule("B3/S23"));
    EXPECT_NO_THROW(Rule("B36/S23"));
    EXPECT_NO_THROW(Rule("B3/S238"));
}

TEST(RuleTest, ConstructorInvalidRules) {
    EXPECT_THROW(Rule("B3S23"), std::invalid_argument);
    EXPECT_THROW(Rule("B3/"), std::invalid_argument);
    EXPECT_THROW(Rule("/S23"), std::invalid_argument);
}

TEST(RuleTest, ShouldLiveConwaysLife) {
    Rule rule("B3/S23");
    EXPECT_TRUE(rule.shouldLive(false, 3));
    EXPECT_FALSE(rule.shouldLive(false, 2));
    EXPECT_FALSE(rule.shouldLive(false, 4));
    EXPECT_TRUE(rule.shouldLive(true, 2));
    EXPECT_TRUE(rule.shouldLive(true, 3));
    EXPECT_FALSE(rule.shouldLive(true, 1));
    EXPECT_FALSE(rule.shouldLive(true, 4));
}

TEST(RuleTest, ShouldLiveCustomRule) {
    Rule rule("B36/S238");
    EXPECT_TRUE(rule.shouldLive(false, 3));
    EXPECT_TRUE(rule.shouldLive(false, 6));
    EXPECT_FALSE(rule.shouldLive(false, 2));
    EXPECT_TRUE(rule.shouldLive(true, 2));
    EXPECT_TRUE(rule.shouldLive(true, 3));
    EXPECT_TRUE(rule.shouldLive(true, 8));
    EXPECT_FALSE(rule.shouldLive(true, 1));
}
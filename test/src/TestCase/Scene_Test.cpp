#include <AE/Graphics.hpp>
#include <memory>
#include <gtest/gtest.h>


TEST(LayerTest, addLayer)
{
    auto scene = std::make_shared<ae::Scene>("MainScene");
    auto layer = std::make_shared<ae::Layer>("MainLayer");

    scene->addLayer(layer);

    ASSERT_EQ(scene->getNumberOfLayers(), 1);
    ASSERT_EQ(layer->isOnScene(), true);
}

TEST(LayerTest, deleteLayer)
{
    auto scene = std::make_shared<ae::Scene>("MainScene");
    auto layer = std::make_shared<ae::Layer>("MainLayer");

    scene->addLayer(layer);
    scene->deleteLayer(layer->getName());

    ASSERT_EQ(scene->getNumberOfLayers(), 0);
    ASSERT_EQ(layer->isOnScene(), false);
}

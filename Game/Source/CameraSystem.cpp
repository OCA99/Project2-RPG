#include "CameraSystem.h"

#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "Window.h"

void CameraSystem::tick(ECS::World* world, float dt)
{
	world->each<CameraFollower>([&](ECS::Entity* entity, ECS::ComponentHandle<CameraFollower> cameraFollower) {
		ECS::ComponentHandle<Position> position = entity->get<Position>();

		app->render->camera.x = -position->position.x * app->win->GetScale() + app->render->camera.w / 2;
		app->render->camera.y = -position->position.y * app->win->GetScale() + app->render->camera.h / 2;

		if ((-app->render->camera.x + app->render->camera.w) / app->win->GetScale() > app->map->OuterRectangle().w)
			app->render->camera.x = -app->map->OuterRectangle().w * app->win->GetScale() + app->render->camera.w;

		if ((-app->render->camera.y + app->render->camera.h) / app->win->GetScale() > app->map->OuterRectangle().h)
			app->render->camera.y = -app->map->OuterRectangle().h * app->win->GetScale() + app->render->camera.h;

		if (app->render->camera.x > 0) app->render->camera.x = 0;
		if (app->render->camera.y > 0) app->render->camera.y = 0;
	});
}